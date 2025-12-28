// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/GASBaseAnimInstance.h"
#include "GASCharacterAnimInstance.generated.h"


class AGASBasicCharacter;
class UCharacterMovementComponent;


/**
 * @brief 角色动画实例基类
 *
 * 继承自UGASBasicAnimInstance，作为所有角色动画实例的基类
 * 负责处理角色通用的动画逻辑，如移动速度计算等
 *
 * @details
 * 1. 管理角色动画的基本属性和方法
 * 2. 提供角色移动相关数据的访问接口
 * 3. 作为更具体的角色动画实例类（如英雄、敌人）的基类
 *
 * @see UWarriorBaseAnimInstance
 */
UCLASS()
class GAS_FIGHT_DEMO_API UGASCharacterAnimInstance : public UGASBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 原生初始化动画函数
	 *
	 * 在动画蓝图初始化时调用，用于初始化动画相关的变量和引用 --> 该函数在动画实例创建时调用一次
	 * 重写父类函数以添加角色特有的初始化逻辑
	 *
	 * @details
	 * 1. 获取拥有此动画实例的Pawn角色对象
	 * 2. 获取角色的移动组件引用
	 */
	virtual void NativeInitializeAnimation() override;

	/**
	 * @brief 原生线程安全更新动画函数
	 *
	 * 在每帧更新时调用，用于更新动画相关的变量
	 * 该函数在线程安全的环境下执行
	 * 重写父类函数以添加角色特有的更新逻辑
	 *
	 * @param DeltaSeconds 距离上次更新经过的时间（秒）
	 *
	 * @details
	 * 1. 更新角色在地面上的移动速度
	 * 2. 检查角色是否有加速度（是否在移动）
	 * 
	 * @note 该函数每帧调用，应避免过于复杂的计算
	 */
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	/**
	 * @brief 拥有的基础角色引用
	 *
	 * 指向当前动画实例所属的基础角色对象
	 * 用于访问角色的属性和方法
	 */
	UPROPERTY()
	TObjectPtr<AGASBasicCharacter> OwningCharacter;

	/**
	 * @brief 拥有的角色移动组件引用
	 *
	 * 指向角色的CharacterMovementComponent组件
	 * 用于获取角色移动相关的信息，如速度、加速度等
	 */
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;

	/**
	 * @brief 地面速度
	 *
	 * 角色在地面上的移动速度（2D平面上的速度大小）
	 * 用于驱动动画混合空间或状态机
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	/**
	 * @brief 是否有加速度
	 *
	 * 标识角色当前是否具有加速度的布尔值
	 * 用于判断角色是否正在主动移动
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
