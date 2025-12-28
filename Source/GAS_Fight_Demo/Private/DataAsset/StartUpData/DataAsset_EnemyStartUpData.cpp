// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUpData/DataAsset_EnemyStartUpData.h"
#include "GAS/FightAbilitySystemComponent.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UFightEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if (!AbilityClass)
			{
				continue;
			}

			// 创建一个技能规格（AbilitySpec），指定技能类（AbilityClass）和应用等级（ApplyLevel），用于描述要赋予的技能
			FGameplayAbilitySpec AbilitySpec(AbilityClass, ApplyLevel);
			// 设置技能的来源对象为当前能力系统组件的 AvatarActor，通常代表技能的拥有者。
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
