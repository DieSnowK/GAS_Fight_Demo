// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"


class AFightPlayerWeapon;


/**
 * 英雄战斗组件类
 *
 * 该组件负责处理英雄角色的战斗相关逻辑，包括武器管理、伤害计算和战斗事件处理等
 * 继承自UPawnCombatComponent，为英雄角色提供专门的战斗功能
 * 主要功能包括：
 * 1. 获取指定标签的英雄武器
 * 2. 获取当前装备的英雄武器
 * 3. 计算当前装备武器在特定等级下的伤害值
 * 4. 处理攻击命中目标的事件
 * 5. 处理武器与目标交互的事件
 */
UCLASS()
class GAS_FIGHT_DEMO_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * 根据武器标签获取英雄携带的武器对象
	 *
	 * @param InWeaponTag 武器标签，用于标识特定的武器类型
	 * @return 返回对应标签的AFightPlayerWeapon对象指针，如果未找到则返回nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|Combat")
	AFightPlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	/**
	 * 获取英雄当前装备的武器对象
	 *
	 * @return 返回当前装备的AFightPlayerWeapon对象指针，如果没有装备武器则返回nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|Combat")
	AFightPlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	/**
	 * 获取英雄当前装备武器在指定等级下的基础伤害值
	 *
	 * @param InLevel 武器等级，用于计算对应等级的伤害值
	 * @return 返回指定等级下的武器基础伤害值
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|Combat")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel) const;

	/**
	 * 当武器命中目标时调用的事件处理函数
	 * 用于处理攻击命中逻辑，如发送命中事件、计算伤害等
	 *
	 * @param HitActor 被命中的目标Actor
	 */
	virtual void OnHitTargetActor(AActor* HitActor) override;

	/**
	 * 当武器从目标上移开时调用的事件处理函数
	 * 用于处理武器与目标交互结束的逻辑
	 *
	 * @param InteractedActor 交互的目标Actor
	 */
	virtual void OnWeaponPulledFromTarget(AActor* InteractedActor) override;
};
