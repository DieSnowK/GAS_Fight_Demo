// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/FightGameplayAbility.h"
#include "FightPlayerGameplayAbility.generated.h"


class AMainCharacter;
class AMainPlayerController;
class UPlayerCombatComponent;


/**
 * @class UFightPlayerGameplayAbility
 * @brief 英雄角色游戏能力类
 *
 * 这是一个专门针对英雄角色的游戏能力类，继承自UFightGameplayAbility
 * 提供了获取英雄角色、英雄控制器和英雄战斗组件的便捷方法
 * 通过缓存机制优化性能，避免重复的对象转换操作
 *
 * 主要功能:
 * 1. 提供获取英雄角色实例的方法
 * 2. 提供获取英雄控制器实例的方法
 * 3. 提供获取英雄战斗组件的方法
 * 4. 使用缓存机制优化性能
 */
UCLASS()
class GAS_FIGHT_DEMO_API UFightPlayerGameplayAbility : public UFightGameplayAbility
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 获取当前能力关联的英雄角色实例
	 *
	 * 该函数通过缓存机制避免重复的Cast操作，提高性能
	 * 如果缓存无效，则从CurrentActorInfo->AvatarActor转换并缓存
	 *
	 * @return AMainCharacter指针，如果无法获取则返回nullptr
	 *
	 * @note BlueprintPure表示该函数不会改变对象状态，可在蓝图中作为Get节点使用
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	AMainCharacter* GetPlayerCharacterFromActorInfo();

	/**
	 * @brief 获取当前能力关联的英雄控制器实例
	 *
	 * 该函数通过缓存机制避免重复的Cast操作，提高性能
	 * 如果缓存无效，则从CurrentActorInfo->PlayerController转换并缓存
	 *
	 * @return AMainPlayerController指针，如果无法获取则返回nullptr
	 *
	 * @note BlueprintPure表示该函数不会改变对象状态，可在蓝图中作为Get节点使用
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	AMainPlayerController* GetPlayerControllerFromActorInfo();

	/**
	 * @brief 获取当前英雄角色的战斗组件
	 *
	 * 该函数是获取英雄角色战斗相关功能的便捷方法
	 * 通过调用GetHeroCharacterFromActorInfo获取英雄角色，然后获取其UPlayerCombatComponent组件
	 *
	 * @return UPlayerCombatComponent指针，用于访问英雄的战斗功能
	 *
	 * @note BlueprintPure表示该函数不会改变对象状态，可在蓝图中作为Get节点使用
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	/**
	 * @brief 获取当前英雄角色的UI组件
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	UPlayerUIComponent* GetPlayerUIComponentFromActorInfo();

	/**
	 * 创建英雄伤害效果的规格句柄
	 *
	 * @param EffectClass 伤害效果的类
	 * @param InWeaponBaseDamage 武器基础伤害值
	 * @param InCurrentAttackTypeTag 当前攻击类型标签
	 * @param InUsedComboCount 已使用的连击次数
	 * @return 包含伤害效果规格的句柄
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, 
		float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	UFUNCTION(BlueprintCallable, Category = "Fight|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldown, float& RemainingCooldown);

private:
	/**
	 * @brief 缓存的英雄角色引用
	 *
	 * 使用TWeakObjectPtr避免循环引用和内存泄漏
	 * TWeakObjectPtr是一种弱引用，当对象被销毁时会自动置为无效状态
	 * 这样可以避免强引用导致的对象无法被垃圾回收
	 */
	TWeakObjectPtr<AMainCharacter> CachedMainCharacter;

	/**
	 * @brief 缓存的英雄控制器引用
	 */
	TWeakObjectPtr<AMainPlayerController> CachedMainPlayerController;
};
