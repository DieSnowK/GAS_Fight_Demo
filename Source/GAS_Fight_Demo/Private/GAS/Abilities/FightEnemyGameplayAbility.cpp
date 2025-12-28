// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/FightEnemyGameplayAbility.h"
#include "Characters/EnemyCharacter.h"

AEnemyCharacter* UFightEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedWarriorEnemyCharacter.IsValid())
	{
		// AvatarActor通常是实际的游戏角色对象 --> CurrentActorInfo->AvatarActor是拥有此能力的角色对象
		CachedWarriorEnemyCharacter = Cast<AEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedWarriorEnemyCharacter.IsValid() ? CachedWarriorEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UFightEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	// 基于上一个函数 --> 方便技能系统访问和操作敌人的战斗功能
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
