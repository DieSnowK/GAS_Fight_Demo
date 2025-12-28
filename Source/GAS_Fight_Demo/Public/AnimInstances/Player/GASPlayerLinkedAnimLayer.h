// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/GASBaseAnimInstance.h"
#include "GASPlayerLinkedAnimLayer.generated.h"


class UGASPlayerAnimInstance;


UCLASS()
class GAS_FIGHT_DEMO_API UGASPlayerLinkedAnimLayer : public UGASBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UGASPlayerAnimInstance* GetPlayerAnimInstance() const;
};
