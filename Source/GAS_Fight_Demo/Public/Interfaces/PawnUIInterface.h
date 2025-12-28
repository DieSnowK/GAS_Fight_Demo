// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnUIInterface.generated.h"


class UPawnUIComponent;
class UPlayerUIComponent;
class UEnemyUIComponent;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnUIInterface : public UInterface
{
	GENERATED_BODY()
};


class GAS_FIGHT_DEMO_API IPawnUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPawnUIComponent* GetPawnUIComponent() const = 0;

	// 其实也可以通过UPawnUIComponent向下转换得到, 只是这样可能使用起来更直观一些
	virtual UPlayerUIComponent* GetPlayerUIComponent() const;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
};
