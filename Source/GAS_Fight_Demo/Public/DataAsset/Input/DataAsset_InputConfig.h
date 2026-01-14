// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "DataAsset_InputConfig.generated.h"


class UInputAction;
class UInputMappingContext;


/**
 * @brief Fighter输入动作配置结构体
 *
 * 用于存储单个输入动作的配置信息，包括关联的Gameplay标签和输入动作对象
 * 该结构体支持蓝图编辑和序列化
 *
 * @details
 * 1. 包含输入标签和输入动作对象的映射关系
 * 2. 提供有效性检查功能
 * 3. 使用USTRUCT宏标记，支持Unreal Engine的反射系统
 */
USTRUCT(BlueprintType)
struct FFightInputActionConfig
{
	GENERATED_BODY()

	FFightInputActionConfig()
		: InputAction(nullptr)
	{}

	/**
	 * @brief 输入标签
	 *
	 * 用于标识输入动作的Gameplay标签
	 * 在编辑器中只能选择"Input"分类下的标签
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag")) // meta = (Categories = "InputTag"): 限制只能选择"InputTag"分类下的标签
	FGameplayTag InputTag;

	/**
	 * @brief 输入动作对象
	 *
	 * 指向实际的输入动作对象
	 * 使用TObjectPtr确保对象引用的正确性
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	/**
	 * @brief 检查配置是否有效
	 *
	 * 验证输入标签和输入动作对象是否都有效
	 *
	 * @return 如果输入标签有效且输入动作对象不为空则返回true，否则返回false
	 */
	bool IsValid() const;
};


/**
 * @brief 输入配置数据资产类
 *
 * 继承自UDataAsset，用于存储和管理游戏的输入配置信息
 * 包含默认映射上下文、原生输入动作和能力输入动作的配置
 *
 * @details
 * 1. 提供输入映射上下文的配置
 * 2. 管理原生输入动作配置列表
 * 3. 管理能力输入动作配置列表
 * 4. 提供根据标签查找输入动作的功能
 *
 * @see UDataAsset
 */
UCLASS()
class GAS_FIGHT_DEMO_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 默认映射上下文
	 *
	 * 用于定义输入设备与游戏行为之间的映射关系
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/**
	 * @brief 原生输入动作配置数组
	 *
	 * 存储所有原生输入动作的配置信息
	 * 在编辑器中以InputTag为标题属性显示
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag")) // 用Input作为此数组的行名称
	TArray<FFightInputActionConfig> NativeInputActions;

	/**
	 * @brief 根据标签查找原生输入动作
	 *
	 * 在原生输入动作配置数组中查找与指定标签匹配的输入动作
	 *
	 * @param InInputTag 要查找的输入标签
	 * @return 找到的输入动作对象指针，未找到则返回nullptr
	 */
	UInputAction* FindNativeInputActionByTag(const FGameplayTag InInputTag) const;

	/**
	 * @brief 能力输入动作配置数组
	 *
	 * 存储所有能力输入动作的配置信息
	 * 在编辑器中以InputTag为标题属性显示
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FFightInputActionConfig> AbilityInputActions;
};
