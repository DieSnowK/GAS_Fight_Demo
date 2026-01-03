// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

/**
 * @brief 定义任务内存结构，用于存储任务执行期间需要保持的数据
 * 
 * BTTask通常需要这样的内存结构来保存跨帧的状态信息
 */
struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const;
	void Reset();
};

UCLASS()
class GAS_FIGHT_DEMO_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_RotateToFaceTarget();

	// ~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	/**
	 * @brief 获取任务需要的内存大小
	 * 
	 * BT系统会分配这个大小的内存来存储任务状态
	 */
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	// ~ End UBTNode Interface

	/**
	 * @brief 行为树任务开始执行时调用
	 * 
	 * @return EBTNodeResult::Type 任务的初始执行结果
	 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/**
	 * @brief 每帧更新任务：当任务返回InProgress时，每帧调用此函数
	 */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/**
	 * @brief 检查AI是否已达到面向目标的角度精度
	 */
	bool HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float AnglePrecision;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector InTargetToFaceKey;
};
