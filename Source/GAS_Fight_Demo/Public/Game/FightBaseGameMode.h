// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FightTypes/FightEnumTypes.h"
#include "FightBaseGameMode.generated.h"


UCLASS()
class GAS_FIGHT_DEMO_API AFightBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFightBaseGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EFightGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EFightGameDifficulty GetCurrentGameDifficulty() const
	{
		return CurrentGameDifficulty;
	}
};
