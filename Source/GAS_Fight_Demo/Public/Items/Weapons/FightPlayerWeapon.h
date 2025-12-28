// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/FightWeaponBase.h"
#include "FightTypes/FightStructTypes.h"
#include "FightPlayerWeapon.generated.h"


/**
 * @brief 英雄武器类
 *
 * 继承自AFightWeaponBase，代表英雄角色使用的武器
 * 扩展了基础武器功能，添加了与英雄能力系统集成的功能
 * 包含英雄武器特定的数据和能力句柄管理
 *
 * @details
 * 1. 集成英雄武器特定数据结构
 * 2. 管理与武器关联的能力规范句柄
 * 3. 支持蓝图访问和调用
 *
 * @see AFightWeaponBase
 */
UCLASS()
class GAS_FIGHT_DEMO_API AFightPlayerWeapon : public AFightWeaponBase
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 英雄武器数据
	 *
	 * 包含英雄武器特有的数据信息
	 * 可在编辑器中配置，默认只能读取
	 * 属于WeaponData分类
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FFightPlayerWeaponData PlayerWeaponData;

	/**
	 * @brief 分配授予的能力规范句柄
	 *
	 * 设置当前武器关联的能力规范句柄数组
	 * 这些句柄用于在使用武器时激活对应的技能能力
	 *
	 * @param InSpecHandles 需要绑定的能力规范句柄数组
	 *
	 * @details
	 * 1. 将传入的句柄数组赋值给内部存储
	 * 2. 这些句柄与武器绑定的能力相关联
	 * 3. 可在蓝图中调用
	 * 4. 直接将传入的句柄数组赋值给GrantedAbilitySpecHandles成员变量
	 * 5. 覆盖之前存储的任何句柄数据
	 * 6. 不验证句柄的有效性，由调用者确保数据正确性
	 */
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	/**
	 * @brief 获取授予的能力规范句柄
	 *
	 * 获取当前武器已绑定的能力规范句柄数组
	 * 用于在武器使用时激活对应技能能力
	 *
	 * @return 返回该武器绑定的能力规范句柄数组
	 *
	 * @details
	 * 1. 返回内部存储的能力规范句柄数组的副本
	 * 2. 该函数为纯函数，不会修改对象状态
	 * 3. 可在蓝图中调用
	 */
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	/**
	 * @brief 授予的能力规范句柄数组
	 *
	 * 存储与该武器关联的GameplayAbility规范句柄
	 * 用于在使用武器时激活对应的技能能力
	 */
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
