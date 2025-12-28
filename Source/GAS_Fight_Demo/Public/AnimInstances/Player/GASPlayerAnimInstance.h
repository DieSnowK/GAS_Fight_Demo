// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/GASCharacterAnimInstance.h"
#include "GASPlayerAnimInstance.generated.h"


class AMainCharacter;


/**
 * @brief 玩家角色动画实例类
 *
 * 继承自UGASPlayerAnimInstance，专门用于英雄角色的动画逻辑处理
 * 负责处理英雄角色特有的动画状态，如休闲状态等
 *
 * @details
 * 1. 管理英雄角色的动画蓝图逻辑
 * 2. 处理角色进入休闲状态的逻辑
 * 3. 继承并扩展基础角色动画实例的功能
 *
 * @see UWarriorCharacterAnimInstance
 */
UCLASS()
class GAS_FIGHT_DEMO_API UGASPlayerAnimInstance : public UGASCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	/**
	 * @brief 拥有的英雄角色引用
	 *
	 * 指向当前动画实例所属的英雄角色对象
	 * 用于访问角色特有的属性和方法
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	TObjectPtr<AMainCharacter> OwningHeroCharacter;

	/**
	 * @brief 是否应该进入休闲状态
	 *
	 * 标识角色是否应该进入休闲动画状态的布尔值
	 * 当角色长时间静止不动时设置为true
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	/**
	 * @brief 进入休闲状态的阈值时间
	 *
	 * 角色保持静止多长时间后进入休闲状态的阈值（秒）
	 * 可在编辑器中调整该值以改变进入休闲状态的时间
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	/**
	 * @brief 空闲经过的时间
	 *
	 * 角色保持无加速度状态（静止）的累计时间（秒）
	 * 用于判断是否达到进入休闲状态的阈值
	 */
	float IdleElapsedTime;
};
