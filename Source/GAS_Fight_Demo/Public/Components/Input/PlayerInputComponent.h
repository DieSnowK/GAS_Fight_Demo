// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "PlayerInputComponent.generated.h"

/**
 * 自定义的输入组件类，继承自UEnhancedInputComponent
 * 提供了绑定原生输入动作和能力输入动作的模板函数
 * 用于处理游戏中的输入逻辑，支持基于GameplayTag的输入映射
 */
UCLASS()
class GAS_FIGHT_DEMO_API UPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * 绑定原生输入动作的模板函数
	 *
	 * @tparam UserObject   用户对象类型（通常是拥有此输入组件的Actor或Controller）
	 * @tparam CallbackFunc 回调函数类型
	 * @param InInputConfig 输入配置数据资产，包含所有输入动作的定义
	 * @param InInputTag    用于标识特定输入动作的GameplayTag
	 * @param TriggerEvent  触发事件类型（如按下、释放、持续按住等）
	 * @param ContextObject 上下文对象，通常是调用此函数的对象实例（如PlayerController）
	 * @param Func          当输入被触发时调用的回调函数
	 */
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, 
		ETriggerEvent TriggerEvent, UserObject* ContextObject, const CallbackFunc& Func);

	/**
	 * 绑定能力输入动作的模板函数
	 *
	 * @tparam UserObject      用户对象类型
	 * @tparam CallbackFunc    回调函数类型
	 * @param InInputConfig    输入配置数据资产
	 * @param ContextObject    上下文对象
	 * @param InputPressedFunc 当输入按下时调用的回调函数
	 * @param InputReleasedFunc 当输入释放时调用的回调函数
	 */
	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,
		CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

template<class UserObject, typename CallbackFunc>
inline void UPlayerInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, 
	const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, const CallbackFunc& Func)
{
	// 检查输入配置数据资产是否有效，如果无效则触发断言
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template<class UserObject, typename CallbackFunc>
inline void UPlayerInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, 
	UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));

	for (const FFightInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid())
		{
			continue;
		}

		// 为输入动作绑定按下事件，传递InputPressedFunc回调函数和对应的InputTag
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, 
			ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		// 为输入动作绑定释放事件，传递InputReleasedFunc回调函数和对应的InputTag
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, 
			ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
