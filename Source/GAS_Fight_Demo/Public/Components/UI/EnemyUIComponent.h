// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UFightWidgetBase;


UCLASS()
class GAS_FIGHT_DEMO_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UFightWidgetBase* InWidgetToRegister);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetIfAny();

private:
	TArray<UFightWidgetBase*> EnemyDrawnWidgets;
};
