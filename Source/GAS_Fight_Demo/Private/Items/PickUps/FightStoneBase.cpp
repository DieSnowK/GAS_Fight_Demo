// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickUps/FightStoneBase.h"
#include "Characters/MainCharacter.h"
#include "GAS/FightGameplayTags.h"
#include "GAS/FightAbilitySystemComponent.h"


void AFightStoneBase::Consume(UFightAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);

	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnStoneConsumed();
}

void AFightStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* OverlappedMainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		OverlappedMainCharacter->GetFightAbilitySystemComponent()->TryActivateAbilityByTag(FightGameplayTags::Player_Ability_PickUp_Stones);
	}
}
