// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/FightPlayerGameplayAbility.h"
#include "Characters/MainCharacter.h"
#include "Controllers/MainPlayerController.h"
#include "GAS/FightAbilitySystemComponent.h"
#include "GAS/FightGameplayTags.h"


AMainCharacter* UFightPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	// 检查缓存的英雄角色是否有效，避免重复获取和转换 --> IsValid()检查弱指针是否仍然指向一个有效的对象
	if (!CachedMainCharacter.IsValid())
	{
		// 从当前ActorInfo的AvatarActor中获取并转换为AMainCharacter --> AvatarActor通常是实际的游戏角色对象
		CachedMainCharacter = Cast<AMainCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedMainCharacter.IsValid() ? CachedMainCharacter.Get() : nullptr;
}

AMainPlayerController* UFightPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedMainPlayerController.IsValid())
	{
		// PlayerController负责处理玩家输入和控制角色行为
		CachedMainPlayerController = Cast<AMainPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedMainPlayerController.IsValid() ? CachedMainPlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UFightPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	// 通过调用GetPlayerCharacterFromActorInfo获取英雄角色，然后获取其PlayerCombatComponent组件
	// PlayerCombatComponent包含了英雄角色的战斗相关逻辑和数据
	// GetPlayerCombatComponent()是AMainCharacter类的方法，返回UPlayerCombatComponent组件
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

UPlayerUIComponent* UFightPlayerGameplayAbility::GetPlayerUIComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent();
}

FGameplayEffectSpecHandle UFightPlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, 
	float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	// 创建效果上下文句柄，用于存储效果的相关信息
	FGameplayEffectContextHandle ContextHandle = GetFightAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	// 设置效果的来源能力
	ContextHandle.SetAbility(this);
	// 添加源对象（通常是角色的Avatar Actor）
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	// 添加引发效果的角色和目标（这里都设置为Avatar Actor）
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 创建传出效果的规格句柄
	FGameplayEffectSpecHandle EffectSpecHandle = GetFightAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass, GetAbilityLevel(), ContextHandle);

	// 设置由调用者指定的基础伤害值
	EffectSpecHandle.Data->SetSetByCallerMagnitude(FightGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);

	// 如果攻击类型标签有效，则设置该标签对应的连击次数
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}

	return EffectSpecHandle;
}

bool UFightPlayerGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, 
	float& TotalCooldown, float& RemainingCooldown)
{
	check(InCooldownTag.IsValid());

	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldown = TimeRemainingAndDuration[0].Key;
		TotalCooldown = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldown > 0.f;
}
