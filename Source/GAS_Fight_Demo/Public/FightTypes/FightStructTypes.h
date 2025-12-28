// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GAS/Abilities/FightPlayerGameplayAbility.h"
//#include "AnimInstances/Player/GASPlayerLinkedAnimLayer.h"
#include "ScalableFloat.h"
#include "FightStructTypes.generated.h"

class UGASPlayerLinkedAnimLayer;
class UInputMappingContext;


/**
 * 英雄能力集合结构体
 *
 * 用于定义和存储英雄武器相关的能力配置信息，包括输入标签和对应的能力类
 * 这个结构体通常用于武器数据资产中，定义武器默认携带的能力集合
 */
USTRUCT(BlueprintType)
struct FFightPlayerAbilitySet
{
	GENERATED_BODY()

	/**
	 * 能力对应的输入标签
	 * 用于将游戏输入映射到特定能力，例如攻击、格挡等
	 * 该标签需要在GameplayTags系统中定义
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Input"))
	FGameplayTag InputTag;

	/**
	 * 需要授予的能力类
	 * 指定与输入标签关联的具体游戏能力类
	 * 当玩家装备该武器时，会自动授予此能力
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UFightPlayerGameplayAbility> AbilityToGrant;

	/**
	 * 检查能力集合是否有效
	 * 验证输入标签和能力类是否都已正确设置
	 *
	 * @return 如果输入标签有效且能力类不为空则返回true，否则返回false
	 */
	bool IsValid() const;
};


USTRUCT(BlueprintType)
struct FFightPlayerWeaponData
{
	GENERATED_BODY()

	/**
	 * 武器动画层类
	 * 指定与该武器关联的动画蓝图类，用于播放武器特定的动画
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGASPlayerLinkedAnimLayer> WeaponAnimLayerToLink;

	/**
	 * 武器输入映射上下文
	 * 指定该武器使用的输入映射上下文，用于处理武器特定的输入操作
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	/**
	 * 武器默认能力集合数组
	 * 包含该武器默认拥有的所有能力配置，每个元素定义一个能力及其对应的输入标签
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FFightPlayerAbilitySet> DefaultWeaponAbilities;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	//TArray<FWarriorHeroSpecialAbilitySet> SpecialWeaponAbilities;

	/**
	 * 武器基础伤害值
	 * 使用可扩展浮点数类型定义武器的基础伤害，支持基于等级的伤害计算
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};