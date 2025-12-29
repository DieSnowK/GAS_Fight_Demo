// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToFaceTarget.h"

bool FRotateToFaceTargetTaskMemory::IsValid() const
{
	return OwningPawn.IsValid() && TargetActor.IsValid();
}

void FRotateToFaceTargetTaskMemory::Reset()
{
	OwningPawn.Reset();
	TargetActor.Reset();
}

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate to Face Target Actor");
	AnglePrecision = 10.0f;
	RotationInterpSpeed = 5.0f;

	bNotifyTick = true;          // 需要每帧Tick更新
	bNotifyTaskFinished = true;  // 任务完成时需要通知
	bCreateNodeInstance = false; // 不为每个AI实例创建独立的节点实例

	INIT_TASK_NODE_NOTIFY_FLAGS();

	// 配置黑板键选择器的过滤器
	// 限制只能选择存储AActor类型对象的黑板键
	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 获取行为树使用的黑板数据
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		// 解析选中的黑板键，建立与黑板数据的关联
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision %s is reached"), 
		*KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const
{
	return false;
}
