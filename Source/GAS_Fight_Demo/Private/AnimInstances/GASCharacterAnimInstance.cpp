// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/GASCharacterAnimInstance.h"
#include "Characters/GASBasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "KismetAnimationLibrary.h"


void UGASCharacterAnimInstance::NativeInitializeAnimation()
{
	// 尝试获取拥有此动画实例的Pawn对象，并转换为AGASBasicCharacter类型
	// TryGetPawnOwner是UAnimInstance提供的函数，用于获取拥有此动画实例的Pawn
	OwningCharacter = Cast<AGASBasicCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		// 获取角色的移动组件引用，用于后续的移动相关数据获取
		// GetCharacterMovement是ACharacter提供的函数，返回CharacterMovementComponent
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UGASCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	// 计算角色在地面上的移动速度（2D平面上的速度大小）-> GetVelocity()返回角色的三维速度向量 -> Size2D()计算二维平面上的速度大小（忽略Z轴）
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	// 检查角色是否具有加速度（是否在主动移动）-> GetCurrentAcceleration()获取当前加速度向量
	// SizeSquared2D()计算二维平面上加速度大小的平方 -> 与0比较平方值避免开方运算，提高性能
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	//LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
