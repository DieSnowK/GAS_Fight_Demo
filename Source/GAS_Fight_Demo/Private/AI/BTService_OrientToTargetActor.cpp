// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_OrientToTargetActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"


UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	// 设置节点名称，这个名称会在行为树编辑器中显示
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	// 初始化服务节点通知标志 --> 这个宏通常设置节点是否需要特定的通知或回调
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationInterpSpeed = 5.0f;
	// 设置服务执行间隔为0，表示每帧都执行
	Interval = 0.f;
	// 随机偏差为0，表示执行时间没有随机变化
	RandomDeviation = 0.f;

	// 配置黑板键选择器的过滤器 --> AddObjectFilter为黑板键添加对象类型过滤器
	// 参数说明：
	// 1. this: 当前对象
	// 2. GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey): 获取成员变量名的安全方式
	// 3. AActor::StaticClass(): 只允许选择AActor类型的对象
	// 这意味着在编辑器中选择黑板键时，只能选择存储Actor引用的键
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 获取行为树使用的黑板数据资产
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		// 解析选中的黑板键，建立与黑板数据的关联 --> 这确保在运行时可以正确访问黑板中存储的值
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	// 获取选中的黑板键的名称
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	// 黑板名称 + 父类的基础服务描述的组合字符串
	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (OwningPawn && TargetActor)
	{
		// 计算从AI当前位置看向目标位置所需的旋转 --> 这个旋转会使AI的正面（X轴正方向）指向目标
		// FindLookAtRotation(起始位置, 目标位置): 返回看向目标所需的旋转值
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(),
			TargetActor->GetActorLocation());

		// 使用插值计算平滑的旋转过渡 --> RInterpTo(当前旋转, 目标旋转, 时间增量, 插值速度)
		// 根据插值速度和时间增量，从当前旋转平滑过渡到目标旋转 --> 这样可以避免AI突然转向，实现自然的旋转动画
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot,
			DeltaSeconds, RotationInterpSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
}
