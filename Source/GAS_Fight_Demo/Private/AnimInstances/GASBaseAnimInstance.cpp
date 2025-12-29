// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/GASBaseAnimInstance.h"
#include "FightFunctionLibrary.h"


bool UGASBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UFightFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}
