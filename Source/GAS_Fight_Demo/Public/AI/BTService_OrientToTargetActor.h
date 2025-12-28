// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GAS_FIGHT_DEMO_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()
	
	UBTService_OrientToTargetActor();

	// ~Begin UBTNode Interface
	/**
	 * @brief 从行为树资产初始化节点
	 * 
	 * @param Asset 行为树资产引用
	 */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	/**
	 * @brief 获取服务的静态描述文本（用于编辑器显示）
	 */
	virtual FString GetStaticDescription() const override;
	// ~End UBTNode Interface

	/**
	 * @brief 行为树服务节点的每帧更新函数
	 * 
	 * 主要功能：让AI角色平滑旋转朝向目标Actor
	 */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 目标Actor的黑板键选择器，用于指定要朝向哪个目标
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	// 旋转插值速度，控制角色旋转朝向目标的速度
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed;
};
