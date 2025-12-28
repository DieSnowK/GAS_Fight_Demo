// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUpData/DataAsset_PlayerStartUpData.h"
#include "GAS/Abilities/FightPlayerGameplayAbility.h"
#include "GAS/FightAbilitySystemComponent.h"


void UDataAsset_PlayerStartUpData::GiveToAbilitySystemComponent(UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const FFightPlayerAbilitySet& AbilitySet : PlayerStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		// 用于存储运行时动态添加的Gameplay Tags --> 与静态配置的标签不同，这些标签可以在游戏运行时动态修改
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		InASCToGive->GiveAbility(AbilitySpec);
	}
}
