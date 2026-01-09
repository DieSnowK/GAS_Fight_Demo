// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FightFunctionLibrary.h"
#include "GAS/FightGameplayTags.h"
#include "Characters/EnemyCharacter.h"
#include "Components/BoxComponent.h"

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

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AEnemyCharacter>();
	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();
	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(
			bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(
			bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	default:
		break;
	}

	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
