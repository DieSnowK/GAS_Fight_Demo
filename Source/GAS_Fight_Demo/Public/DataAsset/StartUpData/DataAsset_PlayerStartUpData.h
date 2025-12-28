// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "FightTypes/FightStructTypes.h"
#include "DataAsset_PlayerStartUpData.generated.h"


UCLASS()
class GAS_FIGHT_DEMO_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FFightPlayerAbilitySet> PlayerStartUpAbilitySets;
};
