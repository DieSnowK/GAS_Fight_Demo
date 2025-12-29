// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/FightAbilitySystemComponent.h"
#include "GAS/FightGameplayTags.h"
#include "GAS/Abilities/FightPlayerGameplayAbility.h"


void UFightAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	// 遍历所有可激活的能力规格
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 检查能力规格的动态标签是否精确匹配输入标签，不匹配则继续下一个能力
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;
		// if (!AbilitySpec.DynamicAbilityTags().HasTagExact(InInputTag)) continue; // 已弃用

		//if (InInputTag.MatchesTag(FightGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
		//{
		//	CancelAbilityHandle(AbilitySpec.Handle);
		//}
		//else
		//{
		//	TryActivateAbility(AbilitySpec.Handle);
		//}
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UFightAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}

void UFightAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FFightPlayerAbilitySet>& InDefaultWeaponAbilities,
	/*const TArray<FWarriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities,*/ int32 ApplyLevel, 
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	// 遍历所有要授予的武器能力集合
	for (const FFightPlayerAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		// 设置能力的源对象为角色的Avatar Actor --> 源对象通常用于效果的上下文信息
		AbilitySpec.SourceObject = GetAvatarActor();
		// 设置能力等级为传入的应用等级 --> 等级会影响能力的效果强度和属性值
		AbilitySpec.Level = ApplyLevel;
		// 将能力集中的输入标签添加到能力的动态标签中 --> 动态标签用于在运行时匹配输入事件
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		// 授予能力并获取能力规格句柄 --> 使用AddUnique确保不会重复添加相同的句柄
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	//for (const FWarriorHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	//{
	//	if (!AbilitySet.IsValid())
	//	{
	//		continue;
	//	}

	//	FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
	//	AbilitySpec.SourceObject = GetAvatarActor();
	//	AbilitySpec.Level = ApplyLevel;
	//	AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
	//	OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	//}
}

void UFightAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(UPARAM(Ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	// 遍历所有要移除的能力规格句柄
	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}

bool UFightAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}

	}

	return false;
}
