// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/FightEnemyGameplayAbility.h"
#include "Characters/EnemyCharacter.h"
#include "GAS/FightGameplayTags.h"
#include "GAS/FightAbilitySystemComponent.h"


AEnemyCharacter* UFightEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedFightEnemyCharacter.IsValid())
	{
		// AvatarActor通常是实际的游戏角色对象 --> CurrentActorInfo->AvatarActor是拥有此能力的角色对象
		CachedFightEnemyCharacter = Cast<AEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedFightEnemyCharacter.IsValid() ? CachedFightEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UFightEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	// 基于上一个函数 --> 方便技能系统访问和操作敌人的战斗功能
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UFightEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetFightAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	// 设置效果的来源能力
	ContextHandle.SetAbility(this);
	// 添加源对象（通常是角色的Avatar Actor）
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	// 添加引发效果的角色和目标（这里都设置为Avatar Actor）
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetFightAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(FightGameplayTags::Shared_SetByCaller_BaseDamage,
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));

	return EffectSpecHandle;
}
