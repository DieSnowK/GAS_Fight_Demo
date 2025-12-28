// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FightTypes/FightEnumTypes.h"
#include "FightGameplayAbility.generated.h"


class UPawnCombatComponent;
class UFightAbilitySystemComponent;


/**
 * @enum EFightAbilityActivationPolicy
 * @brief 定义能力激活策略的枚举类型
 *
 * 该枚举用于控制游戏能力(GameplayAbility)的激活时机和方式
 * 使用UENUM宏标记，使其在Unreal Engine中具有反射信息，支持蓝图、序列化和网络复制
 */
UENUM(BlueprintType)
enum class EFightAbilityActivationPolicy : uint8
{
	/**
	 * OnTriggered - 被动激活策略
	 * 能力需要通过特定的触发机制（如输入事件）来激活
	 * 这是大多数主动技能的默认激活方式
	 * 例如：玩家按下按键释放的技能
	 */
	//OnTriggered UMETA(DisplayName = "On Triggered"),

	/**
	 * OnGiven - 主动激活策略
	 * 能力在被授予给角色时自动激活，无需额外触发
	 * 通常用于被动技能或状态效果
	 * 例如：角色获得时自动生效的增益效果
	 */
	//OnGiven UMETA(DisplayName = "On Given")

	OnTriggered,
	OnGiven
};


/**
 * @class UFightGameplayAbility
 * @brief 游戏能力基类
 *
 * 继承自UGameplayAbility，提供了额外的功能和自定义能力激活逻辑
 * 是所有Fight项目中游戏能力的基类
 *
 * 主要功能:
 * 1. 支持不同的能力激活策略（即时触发 vs 授予时自动激活）
 * 2. 提供便捷的组件访问方法
 * 3. 实现了能力生命周期管理
 * 4. 提供了应用游戏效果到目标的便捷方法
 *
 * @note 该类使用了Unreal Engine的反射系统，支持在蓝图中使用
 */
UCLASS()
class GAS_FIGHT_DEMO_API UFightGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	/**
	 * @brief 在能力被授予给Actor时调用
	 * @param ActorInfo 包含关于拥有此能力的Actor的信息
	 * @param Spec 能力规格，包含关于此能力实例的信息
	 *
	 * 重写父类函数，在能力被添加到AbilitySystemComponent时调用
	 * 根据AbilityActivationPolicy决定是否立即激活能力
	 *
	 * @details
	 * 此函数在能力被添加到AbilitySystemComponent时自动调用
	 * 对于EFightAbilityActivationPolicy::OnGiven策略的能力，会立即尝试激活
	 * 对于EFightAbilityActivationPolicy::OnTriggered策略的能力，需要手动触发
	 */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/**
	 * @brief 在能力结束时调用
	 * @param Handle 能力句柄，用于标识特定的能力实例
	 * @param ActorInfo 包含关于拥有此能力的Actor的信息
	 * @param ActivationInfo 包含关于此能力激活的信息
	 * @param bReplicateEndAbility 是否需要复制能力结束事件到网络上的其他客户端
	 * @param bWasCancelled 能力是否被取消而不是正常结束
	 *
	 * 重写父类函数，在能力完成或被取消时调用
	 * 根据AbilityActivationPolicy决定是否清除能力
	 *
	 * @details
	 * 此函数在能力完成或被取消时自动调用
	 * 对于EFightAbilityActivationPolicy::OnGiven策略的能力，会从ASC中清除
	 * 确保被动能力不会持续占用系统资源
	 */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/**
	 * @brief 能力激活策略属性
	 *
	 * 可在编辑器中设置默认值，控制能力是自动激活还是手动触发
	 * 默认值为EFightAbilityActivationPolicy::OnTriggered
	 *
	 * @note EditDefaultsOnly表示只能在类默认值中编辑，不能在实例中编辑
	 * @see EFightAbilityActivationPolicy
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Fight|Ability")
	EFightAbilityActivationPolicy AbilityActivationPolicy = EFightAbilityActivationPolicy::OnTriggered;

	/**
	 * @brief 获取Pawn战斗组件的便捷方法
	 * @return UPawnCombatComponent指针，如果不存在则返回nullptr
	 *
	 * 蓝图可调用的纯函数，用于获取PawnCombatComponent组件
	 * 纯函数意味着它不会改变对象状态，可用于蓝图中的Get节点
	 *
	 * @details
	 * 通过ActorInfo获取AvatarActor，然后查找其上的UPawnCombatComponent组件
	 * PawnCombatComponent通常负责处理与Pawn相关的战斗逻辑
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	/**
	 * @brief 获取Fighter能力系统组件的便捷方法
	 * @return UFightAbilitySystemComponent指针，如果不存在或类型不匹配则返回nullptr
	 *
	 * 蓝图可调用的纯函数，用于获取FightAbilitySystemComponent组件
	 * 纯函数意味着它不会改变对象状态，可用于蓝图中的Get节点
	 *
	 * @details
	 * 从CurrentActorInfo中获取AbilitySystemComponent并转换为UFightAbilitySystemComponent类型
	 * UFightAbilitySystemComponent是项目特定的扩展，提供了额外的功能
	 */
	UFUNCTION(BlueprintPure, Category = "Fight|Ability")
	UFightAbilitySystemComponent* GetFightAbilitySystemComponentFromActorInfo() const;

	/**
	 * @brief 将游戏效果规格应用到目标Actor的原生实现
	 * @param TargetActor 目标Actor，效果将应用到该Actor上
	 * @param InSpecHandle 游戏效果规格句柄，包含要应用的效果信息
	 * @return FActiveGameplayEffectHandle 活动的游戏效果句柄，可用于后续操作如移除效果
	 *
	 * 原生C++函数，用于将指定的游戏效果应用到目标Actor
	 * 包含了必要的有效性检查以避免崩溃
	 *
	 * @details
	 * 1. 获取目标Actor的AbilitySystemComponent
	 * 2. 验证目标ASC和效果规格的有效性
	 * 3. 应用效果到目标ASC
	 * 4. 返回活动效果句柄
	 */
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(
		AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	/**
	 * @brief 将游戏效果规格应用到目标Actor的蓝图可调用版本
	 * @param TargetActor 目标Actor，效果将应用到该Actor上
	 * @param InSpecHandle 游戏效果规格句柄，包含要应用的效果信息
	 * @param OutSuccessType 输出参数，指示操作是否成功
	 * @return FActiveGameplayEffectHandle 活动的游戏效果句柄，可用于后续操作如移除效果
	 *
	 * 蓝图可调用函数，封装了NativeApplyEffectSpecHandleToTarget并提供执行结果反馈
	 * 使用ExpandEnumAsExecs元数据使SuccessType枚举在蓝图中显示为执行引脚
	 *
	 * @details
	 * 1. 调用NativeApplyEffectSpecHandleToTarget执行实际应用逻辑
	 * 2. 根据返回的句柄判断操作是否成功
	 * 3. 设置输出参数以在蓝图中提供反馈
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, 
		const FGameplayEffectSpecHandle& InSpecHandle, EFightSuccessType& OutSuccessType);
};
