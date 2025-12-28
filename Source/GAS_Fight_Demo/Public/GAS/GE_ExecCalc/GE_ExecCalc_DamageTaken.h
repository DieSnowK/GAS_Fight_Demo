// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GE_ExecCalc_DamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class GAS_FIGHT_DEMO_API UGE_ExecCalc_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 构造函数，初始化游戏效果执行计算所需的属性捕获定义
	 *
	 * 将所有需要捕获的属性定义添加到相关属性列表中，供执行计算时使用
	 * 这里使用了优化的方法，通过静态函数获取预定义的捕获定义
	 *
	 * @details
	 * 1. 添加攻击力属性捕获定义到相关属性列表中
	 * 2. 添加防御力属性捕获定义到相关属性列表中
	 * 3. 添加伤害承受属性捕获定义到相关属性列表中
	 * 4. 这些属性将在执行计算时被使用
	 */
	UGE_ExecCalc_DamageTaken();

	/**
	 * @brief 执行计算实现函数，这是游戏效果执行时调用的核心函数
	 *
	 * 用于计算伤害值并输出结果，是整个伤害计算逻辑的核心实现
	 *
	 * @param ExecutionParams 执行参数，包含源和目标的信息，如属性值、标签等
	 * @param OutExecutionOutput 执行输出，用于输出计算结果，如修改后的属性值
	 *
	 * @details
	 * 1. 获取效果规格说明和相关参数
	 * 2. 创建聚合评估参数用于属性值计算
	 * 3. 获取攻击方攻击力和防御方防御力
	 * 4. 解析调用者设置的基础伤害和攻击类型信息
	 * 5. 根据攻击类型和连击次数计算伤害增幅
	 * 6. 应用伤害计算公式得出最终伤害值
	 * 7. 将最终伤害值作为输出修饰符添加到执行输出中
	 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
