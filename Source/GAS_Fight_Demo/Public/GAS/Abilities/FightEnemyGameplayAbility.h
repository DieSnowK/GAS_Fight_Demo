// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/FightGameplayAbility.h"
#include "FightEnemyGameplayAbility.generated.h"


/**
 * @brief 敌人角色游戏能力类
 *
 * 专门针对敌人角色的游戏能力类，继承自UFightGameplayAbility
 * 提供了获取敌人角色和敌人战斗组件的便捷方法
 * 通过缓存机制优化性能，避免重复的对象转换操作
 *
 * 主要功能:
 * 1. 提供获取敌人角色实例的方法
 * 2. 提供获取敌人战斗组件的方法
 * 3. 使用缓存机制优化性能
 */
UCLASS()
class GAS_FIGHT_DEMO_API UFightEnemyGameplayAbility : public UFightGameplayAbility
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 获取敌人角色实例的函数
	 *
	 * 通过缓存机制获取当前能力关联的敌人角色实例
	 * 如果缓存无效，则从CurrentActorInfo->AvatarActor转换并缓存
	 *
	 * @return AEnemyCharacter指针，如果无法获取则返回nullptr
	 *
	 * @note BlueprintPure表示该函数不会改变对象状态，可在蓝图中作为Get节点使用
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	AEnemyCharacter* GetEnemyCharacterFromActorInfo();

	/**
	 * @brief 获取敌人战斗组件的函数
	 *
	 * 该函数是获取敌人角色战斗相关功能的便捷方法
	 * 通过调用GetEnemyCharacterFromActorInfo获取敌人角色，然后获取其EnemyCombatComponent组件
	 *
	 * @return UEnemyCombatComponent指针，用于访问敌人的战斗功能
	 *
	 * @note BlueprintPure表示该函数不会改变对象状态，可在蓝图中作为Get节点使用
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	/**
	 * @brief 缓存的敌人角色引用
	 *
	 * 使用TWeakObjectPtr避免循环引用和内存泄漏
	 * TWeakObjectPtr是一种弱引用，当对象被销毁时会自动置为无效状态
	 * 这样可以避免强引用导致的对象无法被垃圾回收
	 * TWeakObjectPtr用于引用 UObject 派生对象但不拥有其生命周期，即不会阻止对象被销毁
	 */
	TWeakObjectPtr<AEnemyCharacter> CachedWarriorEnemyCharacter;
};
