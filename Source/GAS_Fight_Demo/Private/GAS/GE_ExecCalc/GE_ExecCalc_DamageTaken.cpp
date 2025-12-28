// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GE_ExecCalc/GE_ExecCalc_DamageTaken.h"
#include "GAS/BasicAttributeSet.h"
#include "GAS/FightGameplayTags.h"

#include "GASDebugHelper.h"

/**
 * @brief 用于定义和捕获与伤害计算相关的属性的结构体
 *
 * 该结构体封装了伤害计算所需的所有属性捕获定义，包括攻击方的攻击力、
 * 防御方的防御力以及最终伤害值。使用静态实例确保只创建一次，提高性能。
 *
 * @details
 * 1. DECLARE_ATTRIBUTE_CAPTUREDEF宏 声明属性捕获定义
 * 2. DEFINE_ATTRIBUTE_CAPTUREDEF宏 定义具体的属性捕获参数
 * 3. 包含了计算伤害所需的所有相关属性定义
 */
struct FFightDamageCapture
{
	// 声明攻击力属性捕获定义 --> 用于捕获攻击方的攻击力属性
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);

	/**
	 * @brief 构造函数，定义属性捕获的具体参数
	 *
	 * 使用宏来简化属性捕获定义的过程，为每个属性设置捕获源和捕获时机
	 *
	 * @details
	 * 1. AttackPower从Source（攻击方）获取，不在执行时捕获
	 * 2. DefensePower从Target（防御方）获取，不在执行时捕获
	 * 3. DamageTaken从Target（防御方）获取，不在执行时捕获
	 * 4. 所有属性都使用UBasicAttributeSet作为属性集类
	 */
	FFightDamageCapture()
	{
		// 参数说明：属性集类、属性名、捕获源(Source/Target)、是否在执行时捕获
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, DamageTaken, Target, false);
	}
};


/**
 * @brief 获取静态的FFightDamageCapture实例
 *
 * 使用局部静态变量确保只创建一次实例，提高性能并保证线程安全
 * 这是C++中单例模式的一种实现方式
 *
 * @return 返回FFightDamageCapture的引用
 *
 * @details
 * 1. 使用局部静态变量确保延迟初始化
 * 2. 保证线程安全性（C++11标准保证）
 * 3. 避免多次创建和销毁对象的开销
 */
static const FFightDamageCapture& GetFightDamageCapture()
{
	// 局部静态变量，C++11标准保证线程安全的延迟初始化 --> 只在第一次调用时创建实例，后续调用直接返回已创建的实例
	static FFightDamageCapture FightDamageCapture;
	return FightDamageCapture;
}


UGE_ExecCalc_DamageTaken::UGE_ExecCalc_DamageTaken()
{
	// 添加攻击力属性捕获定义到相关属性列表中
	RelevantAttributesToCapture.Add(GetFightDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetFightDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetFightDamageCapture().DamageTakenDef);
}

void UGE_ExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 获取拥有此效果的规格说明，包含所有效果相关的信息
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
#pragma region MaybeUsefulFunctions
	// EffectSpec.GetContext().GetSourceObject();
	// EffectSpec.GetContext().GetAbility();
	// EffectSpec.GetContext().GetInstigator();
	// EffectSpec.GetContext().GetEffectCauser();
#pragma endregion

	// 创建聚合评估参数，用于属性值的计算 --> 这些参数将用于获取捕获的属性值
	FAggregatorEvaluateParameters EvaluationParameters;
	// 设置源标签，用于属性捕获时的标签匹配
	EvaluationParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 获取源的攻击力数值
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetFightDamageCapture().AttackPowerDef, EvaluationParameters, SourceAttackPower);

	// 初始化基础伤害值和攻击连击计数 --> 这些值将从调用者设置的标签数值中获取
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// 遍历所有由调用者设置的标签数值，获取基础伤害和攻击类型信息 --> SetByCallerTagMagnitudes允许在应用效果时动态设置属性值
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		// 检查是否为基本伤害标签 --> Shared_SetByCaller_BaseDamage标签用于标识基础伤害值
		if (TagMagnitude.Key.MatchesTagExact(FightGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			// 设置基础伤害值 --> 从标签对应的数值中获取基础伤害
			BaseDamage = TagMagnitude.Value;
		}

		// 检查是否为轻攻击类型标签 --> Player_SetByCaller_AttackType_Light标签用于标识轻攻击及其连击次数
		if (TagMagnitude.Key.MatchesTagExact(FightGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			// 设置轻攻击连击次数 --> 数值表示当前轻攻击的连击计数
			UsedLightAttackComboCount = TagMagnitude.Value;
		}
		else if (TagMagnitude.Key.MatchesTagExact(FightGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}
	}

	// 获取目标的防御力数值
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetFightDamageCapture().DefensePowerDef, EvaluationParameters, TargetDefensePower);

	// 如果有轻攻击连击，则计算伤害增加百分比 --> 轻攻击连击会提供递增的伤害加成
	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;
		BaseDamage *= DamageIncreasePercentLight;
	}
	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;
		BaseDamage *= DamageIncreasePercentHeavy;
	}

	// 计算最终伤害值 --> 伤害公式：最终伤害 = 基础伤害 * 攻击方攻击力 / 防御方防御力
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;

	// 如果最终伤害值大于0，则将其作为修饰符添加到执行输出中 --> 只有正伤害才会被应用到目标角色
	if (FinalDamageDone > 0.f)
	{
		// 添加输出修饰符，将计算出的伤害值应用到目标的DamageTaken属性上 --> 使用Override操作覆盖目标属性值
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UBasicAttributeSet::GetDamageTakenAttribute(), EGameplayModOp::Override, FinalDamageDone));
	}
}
