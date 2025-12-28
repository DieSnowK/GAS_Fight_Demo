// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/GASPlayerAnimInstance.h"
#include "Characters/MainCharacter.h"


/**
 * @brief 原生初始化动画函数实现
 *
 * 在动画蓝图初始化时调用，用于初始化动画相关的变量和引用
 * 重写父类函数以添加英雄角色特有的初始化逻辑
 *
 * @details
 * 1. 调用父类的NativeInitializeAnimation函数执行基础初始化
 * 2. 检查OwningCharacter是否有效
 * 3. 将OwningCharacter转换为AWarriorHeroCharacter并赋值给OwningHeroCharacter
 *
 * @note 依赖于父类已初始化的OwningCharacter指针
 */
void UGASPlayerAnimInstance::NativeInitializeAnimation()
{
	// 调用父类的初始化函数，确保基础动画初始化完成
	Super::NativeInitializeAnimation();

	if (OwningCharacter) // 依赖于父类已初始化的OwningCharacter指针
	{
		// 将基础角色对象转换为英雄角色对象并保存引用
		OwningHeroCharacter = Cast<AMainCharacter>(OwningCharacter);
	}
}

void UGASPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	// 调用父类的更新函数，确保基础动画更新完成
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}
