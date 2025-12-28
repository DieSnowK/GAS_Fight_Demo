// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_FIGHT_DEMO_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template <class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, 
			"'T' Template Parameter GetPawn Must Be Derived From APawn");
		return CastChecked<T>(GetOwner());
	}

	// 非模板重载
	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template <class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, 
			"'T' Template Parameter to GetController Must Be Derived From AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
