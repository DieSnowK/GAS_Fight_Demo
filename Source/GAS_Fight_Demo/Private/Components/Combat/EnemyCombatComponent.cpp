// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FightFunctionLibrary.h"
#include "GAS/FightGameplayTags.h"
#include "FightFunctionLibrary.h"

#include "GASDebugHelper.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	OverlappedActors.AddUnique(HitActor);

	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UFightFunctionLibrary::NativeDoesActorHaveTag(
		HitActor, FightGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UFightFunctionLibrary::NativeDoesActorHaveTag(
		GetOwningPawn(), FightGameplayTags::Enemy_Status_Unblockable);

	// 检测Block是否有效
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UFightFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor,
			FightGameplayTags::Player_Event_SuccessfulBlock, EventData);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),
			FightGameplayTags::Shared_Event_MeleeHit, EventData);
	}
}
