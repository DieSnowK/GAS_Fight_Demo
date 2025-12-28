// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "GAS/Abilities/FightEnemyGameplayAbility.h"
#include "DataAsset_EnemyStartUpData.generated.h"


class UFightAbilitySystemComponent;


UCLASS()
class GAS_FIGHT_DEMO_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	/**
	 * TSubclassOf让数组只存储 UFightEnemyGameplayAbility 及其子类类型（不是实例）
	 * 这样可以在编辑器中选择和配置不同的技能类，运行时再动态创建对应的技能对象。
	 * 如果不加TSubclassOf，则表示数组里存放的是技能对象的实例，而不是类类型
	 */
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFightEnemyGameplayAbility>> EnemyCombatAbilities;
};
