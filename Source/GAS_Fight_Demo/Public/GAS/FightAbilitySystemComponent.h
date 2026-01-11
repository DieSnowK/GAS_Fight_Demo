// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FightTypes/FightStructTypes.h"
#include "FightAbilitySystemComponent.generated.h"


/**
 * UFightAbilitySystemComponent类
 *
 * 自定义的能力系统组件，继承自UAbilitySystemComponent
 * 负责处理游戏中的能力系统逻辑，包括能力的授予、输入处理等
 */
UCLASS()
class GAS_FIGHT_DEMO_API UFightAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 处理能力输入按下的函数
	 *
	 * 当玩家按下与特定GameplayTag关联的输入时调用此函数
	 * 遍历所有可激活的能力，查找与输入标签匹配的能力并尝试激活
	 *
	 * @param InInputTag 与按下输入相关联的GameplayTag，用于匹配对应的能力
	 *
	 * @details
	 * 1. 首先验证输入标签的有效性
	 * 2. 遍历所有可激活的能力规格
	 * 3. 检查能力的动态标签是否包含指定的输入标签
	 * 4. 如果匹配，则尝试激活该能力
	 *
	 * @note 使用GetDynamicSpecSourceTags()替代已弃用的DynamicAbilityTags()方法
	 */
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);

	/**
	 * @brief 处理能力输入释放的函数
	 *
	 * 当玩家释放与特定GameplayTag关联的输入时调用此函数
	 * 目前为空实现，待后续完善
	 *
	 * @param InInputTag 与释放输入相关联的GameplayTag
	 *
	 * @details
	 * 该函数用于处理能力的输入释放逻辑，例如:
	 * 1. 取消持续性能力
	 * 2. 触发释放相关的效果
	 * 3. 更新能力状态
	 *
	 * @todo 实现完整的输入释放逻辑
	 */
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	/**
	 * @brief 授予英雄武器能力的函数
	 *
	 * 根据传入的武器能力集合，为角色授予相应的游戏能力
	 * 这些能力通常与特定武器相关联，在装备武器时调用
	 *
	 * @param InDefaultWeaponAbilities 要授予的武器能力集合，包含输入标签和能力类的映射关系
	 * @param ApplyLevel 能力应用的等级，影响能力的效果强度
	 * @param OutGrantedAbilitySpecHandles 输出参数，返回授予的能力规格句柄数组，用于后续管理这些能力
	 *
	 * @details
	 * 1. 检查传入的能力集合是否为空
	 * 2. 遍历能力集合中的每个能力配置
	 * 3. 验证能力配置的有效性
	 * 4. 为每个有效能力创建能力规格实例
	 * 5. 设置能力的源对象、等级和动态标签
	 * 6. 授予能力并保存能力句柄
	 *
	 * @note 使用GetDynamicSpecSourceTags()替代已弃用的DynamicAbilityTags()方法
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|Ability", meta = (ApplyLevel = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FFightPlayerAbilitySet>& InDefaultWeaponAbilities,
		const TArray<FFightPlayerSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel,
		TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles); // 在暴露给蓝图或需要持久化的变量中使用 int32

	/**
	 * 移除已授予的英雄武器能力
	 *
	 * 清除之前授予的角色武器能力，通常在卸载武器时调用
	 * 
	 * @param InSpecHandlesToRemove 要移除的能力规格句柄数组的引用
	 * 通过 UPARAM(Ref) 声明InSpecHandlesToRemove以引用方式传递 允许函数修改外部数组
	 * 
	 * @details
	 * 1. 检查要移除的能力句柄数组是否为空
	 * 2. 遍历所有能力句柄
	 * 3. 验证句柄的有效性
	 * 4. 清除对应的能力
	 * 5. 清空句柄数组
	 *
	 * @note 函数执行完成后会清空传入的句柄数组
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|Ability")
	void RemovedGrantedHeroWeaponAbilities(UPARAM(Ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Fight|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
