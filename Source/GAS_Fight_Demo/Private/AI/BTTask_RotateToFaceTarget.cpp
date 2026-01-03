// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToFaceTarget.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include"BehaviorTree/BlackboardComponent.h"


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
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 将NodeMemory指针转换为自定义内存结构 --> CastInstanceNodeMemory是UBTTaskNode提供的安全类型转换函数
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	// 将当前状态存储到任务内存中（供TickTask使用）
	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;

	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	// 检查是否已经达到目标角度精度 --> 如果已经基本面向目标，重置内存并返回成功
	if (HasReachedAnglePrecision(OwningPawn, TargetActor))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	// 任务需要继续执行，返回进行中状态 --> 行为树系统随后会开始调用TickTask
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 获取任务内存数据
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	// 如果数据无效，结束任务并返回失败 --> FinishLatentTask用于结束进行中的任务
	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 检查是否已达到角度精度 --> 已达到精度要求，重置内存并结束任务（成功）
	if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	// 还未达到精度，执行旋转逻辑
	else
	{
		// 计算从AI位置看向目标位置所需的旋转
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		// 使用插值计算平滑的旋转过渡
		const FRotator TargetRot = FMath::RInterpTo(
			Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		// 应用计算出的旋转到AI角色
		Memory->OwningPawn->SetActorRotation(TargetRot);

		// 注意：没有调用FinishLatentTask，任务继续执行（下一帧继续TickTask）
	}
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const
{
	// 获取AI角色的前向向量（当前朝向）
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	// 计算从AI到目标的归一化方向向量 --> GetSafeNormal(): 获取单位向量（长度为1），自动处理零向量情况
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	// 计算两个向量的点积 --> (A·B = |A||B|cosθ) --> 因为两个都是单位向量（长度为1），所以点积等于cosθ
	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);

	// 通过反余弦函数计算角度（弧度转角度）--> (θ = arccos(A·B))
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

	// 比较当前角度差与预设精度 --> 如果角度差小于等于精度要求，返回true（已足够面向目标）
	return AngleDiff <= AnglePrecision;
}
