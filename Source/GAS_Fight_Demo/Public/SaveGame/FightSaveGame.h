// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FightTypes/FightEnumTypes.h"
#include "FightSaveGame.generated.h"


UCLASS()
class GAS_FIGHT_DEMO_API UFightSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EFightGameDifficulty SavedCurrentGameDifficulty;
};
