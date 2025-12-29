// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FightTypes/FightEnumTypes.h"
#include "FightFunctionLibrary.generated.h"



class UFightAbilitySystemComponent;


/**
 * 战士功能库类 - 提供各种通用的静态函数供蓝图和C++使用
 * 继承自 UBlueprintFunctionLibrary，所有函数都是静态的，可以在任何地方调用
 * 主要功能包括:
 * 1. 获取角色的能力系统组件
 * 2. 管理角色的 Gameplay 标签
 * 3. 获取角色的战斗组件
 * 这些函数简化了在蓝图中访问和操作游戏系统组件的过程
 */
UCLASS()
class GAS_FIGHT_DEMO_API UFightFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * 从指定 Actor 获取 Fighter 能力系统组件的原生函数
	 * 这是一个辅助函数，用于在 C++ 代码中快速获取 UFightAbilitySystemComponent
	 *
	 * @param InActor 需要获取能力系统组件的 Actor 对象
	 * @return 返回 Cast 后的 UFightAbilitySystemComponent 指针，如果 Actor 不存在或组件不存在则返回 nullptr
	 * @note 此函数为纯 C++ 函数，不直接暴露给蓝图使用
	 */
	static UFightAbilitySystemComponent* NativeGetFighterASCFromActor(AActor* InActor);

	/**
	 * 用于管理 Actor 上的 Gameplay 标签
	 * 检查 Actor 是否已经拥有指定标签，如果没有则添加该标签
	 * 这个函数避免了重复添加相同标签的问题
	 *
	 * @param InActor 需要添加标签的 Actor 对象
	 * @param TagToAdd 需要添加的 Gameplay 标签
	 * @note 此函数在蓝图中可调用，分类为 "Fight|FunctionLibrary"
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/**
	 * 从 Actor 上移除指定的 Gameplay 标签（如果存在）
	 * 这个函数用于清理不再需要的标签，避免标签冗余
	 *
	 * @param InActor 需要移除标签的 Actor 对象
	 * @param TagToRemove 需要移除的 Gameplay 标签
	 * @note 此函数在蓝图中可调用，分类为 "Fight|FunctionLibrary"
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	/**
	 * 原生函数：检查 Actor 是否具有指定的游戏标签
	 * 这是一个辅助函数，用于在 C++ 代码中快速检查 Actor 是否拥有特定标签
	 *
	 * @param InActor 需要检查标签的 Actor 对象
	 * @param TagToCheck 需要检查的 Gameplay 标签
	 * @return 如果 Actor 具有指定标签返回 true，否则返回 false
	 * @note 此函数为纯 C++ 函数，不直接暴露给蓝图使用
	 */
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	/**
	 * 蓝图函数：检查 Actor 是否具有指定的游戏标签，使用枚举扩展执行引脚
	 * 通过 ExpandEnumAsExecs 元数据，此函数可以根据检查结果提供不同的执行路径
	 * 这使得在蓝图中可以根据标签存在与否执行不同的逻辑分支
	 *
	 * @param InActor 需要检查标签的 Actor 对象
	 * @param TagToCheck 需要检查的 Gameplay 标签
	 * @param OutConfirmResult 输出结果枚举，根据是否有标签返回 Yes 或 No
	 * @note 此函数在蓝图中可调用，显示名称为 "Does Actor Have Tag"，分类为 "Fight|FunctionLibrary"
	 *       通过 ExpandEnumAsExecs 元数据扩展了执行引脚，提供 Yes 和 No 两个执行路径
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmResult"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EFightConfirmType& OutConfirmResult);

	/**
	 * 从 Actor 获取 Pawn 战斗组件的原生函数
	 * 这是一个辅助函数，用于在 C++ 代码中快速获取 UPawnCombatComponent
	 *
	 * @param InActor 需要获取战斗组件的 Actor 对象
	 * @return 返回 UPawnCombatComponent 指针，如果不存在则返回 nullptr
	 * @note 此函数为纯 C++ 函数，不直接暴露给蓝图使用
	 */
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	/**
	 * 蓝图函数：从 Actor 获取 Pawn 战斗组件，使用有效性枚举扩展执行引脚
	 * 通过 ExpandEnumAsExecs 元数据，此函数可以根据组件是否存在提供不同的执行路径
	 * 这使得在蓝图中可以根据组件存在与否执行不同的逻辑分支
	 *
	 * @param InActor 需要获取战斗组件的 Actor 对象
	 * @param OutValidType 输出结果枚举，根据组件是否有效返回 Valid 或 Invalid
	 * @return 返回 UPawnCombatComponent 指针
	 * @note 此函数仅为蓝图使用设计，在蓝图中显示名称为 "Get Pawn Combat Component From Actor"
	 *       通过 ExpandEnumAsExecs 元数据扩展了执行引脚，提供 Valid 和 Invalid 两个执行路径
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EFightValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Fight|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);
};
