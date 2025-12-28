// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"


class UFightGameplayAbility;
class UFightAbilitySystemComponent;
class UGameplayEffect;


/**
 * @brief 启动数据基础类
 *
 * 继承自UDataAsset，作为所有启动数据资产的基类
 * 负责在游戏开始时初始化角色的能力系统组件
 * 包括授予能力、应用游戏效果等操作
 *
 * @details
 * 1. 管理需要在启动时授予的能力
 * 2. 管理需要在启动时应用的游戏效果
 * 3. 提供将数据应用到能力系统组件的接口
 *
 * @see UDataAsset
 */
UCLASS()
class GAS_FIGHT_DEMO_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 将启动数据应用到能力系统组件
	 *
	 * 将数据资产中配置的能力和效果应用到指定的能力系统组件
	 * 这是启动数据资产的核心功能函数
	 *
	 * @param InASCToGive 指向要应用数据的能力系统组件指针
	 * @param ApplyLevel 应用等级，默认为1级
	 *
	 * @details
	 * 1. 验证能力系统组件指针的有效性
	 * 2. 授予激活类能力和反应类能力
	 * 3. 应用启动时的游戏效果
	 */
	virtual void GiveToAbilitySystemComponent(UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	/**
	 * @brief 激活时授予的能力数组
	 *
	 * 存储在给出后立即激活的能力类
	 * 这些能力会在角色初始化时自动激活
	 */
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFightGameplayAbility>> ActivateOnGivenAbilities;

	/**
	 * @brief 反应类能力数组
	 *
	 * 存储在特定条件下触发的能力，例如命中反应或死亡之类的能力
	 * 这些能力通常需要特定条件触发才会激活
	 */
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UFightGameplayAbility>> ReactiveAbilities;

	/**
	 * @brief 启动时应用的游戏效果数组
	 *
	 * 存储在角色初始化时需要应用的游戏效果类
	 * 这些效果通常用于初始化角色的属性值
	 */
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	/**
	 * @brief 授予能力的辅助函数
	 *
	 * 将指定数组中的能力授予给指定的能力系统组件
	 *
	 * @param InAbilitiesToGive 要授予的能力类数组
	 * @param InASCToGive 目标能力系统组件指针
	 * @param ApplyLevel 能力应用等级，默认为1级
	 *
	 * @details
	 * 1. 检查能力数组是否为空
	 * 2. 遍历能力数组，为每个有效能力创建能力规范
	 * 3. 设置能力规范的源对象和等级
	 * 4. 将能力规范授予给能力系统组件
	 */
	void GrantAbilities(const TArray<TSubclassOf<UFightGameplayAbility>>& InAbilitiesToGive,
		UFightAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
