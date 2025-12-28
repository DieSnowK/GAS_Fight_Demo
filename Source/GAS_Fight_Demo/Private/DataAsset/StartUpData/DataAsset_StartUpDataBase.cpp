// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "GAS/FightAbilitySystemComponent.h"
#include "GAS/Abilities/FightGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			// 创建一个GameplayEffectSpecHandle实例，使用指定的EffectClass和ApplyLevel
			// MakeOutgoingSpec用于创建一个效果规范句柄，包含效果类、等级和效果上下文
			FGameplayEffectSpecHandle EffectSpecHandle = InASCToGive->MakeOutgoingSpec(EffectClass, ApplyLevel, InASCToGive->MakeEffectContext());
			if (EffectSpecHandle.IsValid())
			{
				// 应用效果规范到自身，而不是直接使用EffectCDO --> 这样可以确保所有属性都被正确初始化和处理
				 if (EffectSpecHandle.Data.IsValid() && EffectSpecHandle.Data.Get())
				 {
				 	InASCToGive->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
				 }

#pragma region Deprecated implementation
			   /* 以下为原始实现方式（已废弃）
				* // 通过 EffectClass（一个 UGameplayEffect 的子类类型），获取该类的默认对象 --> 即该效果的模板实例
				* UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
				* if (EffectCDO)
				* {
				* 	// 调用能力系统组件的 ApplyGameplayEffectToSelf 方法
				* 	// 将上一步获取到的 GameplayEffect 默认对象以 ApplyLevel 等级和指定的效果上下文（EffectContext）应用到自己身上
				* 	InASCToGive->ApplyGameplayEffectToSelf(
				* 		EffectCDO,
				* 		ApplyLevel,
				* 		InASCToGive->MakeEffectContext()
				* 	);
				* }
				*/
#pragma endregion
			}
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UFightGameplayAbility>>& InAbilitiesToGive,
	UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	// 遍历要授予的能力数组
	for (const TSubclassOf<UFightGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability)
		{
			continue;
		}

		// 创建能力规范，使用能力类作为参数
		FGameplayAbilitySpec AbilitySpec(Ability);
		// 设置能力规范的源对象为能力系统组件的AvatarActor
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		// 设置能力规范的等级
		AbilitySpec.Level = ApplyLevel;

		// 将能力规范授予给能力系统组件 --> GiveAbility会将能力添加到能力系统组件中，使其可以被激活和使用
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
