// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/FightAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GASDebugHelper.h"


AFightAIController::AFightAIController(const FObjectInitializer& ObjectInitializer)
	// override PathFollowingComponent to use CrowdFollowingComponent
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// 创建一个UAISenseConfig_Sight类型的视觉感知配置对象
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");

	// 设置该视觉配置的检测关系 --> 按隶属关系检测
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;    // 检测敌人
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; // 不检测友方单位
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false; // 不检测中立单位

	// 设置视觉感知的半径：AI可以看到的最大距离（单位：厘米）
	AISenseConfig_Sight->SightRadius = 5000.0f;

	// 设置失去视野的半径：通常比SightRadius小，用于提供缓冲区域 --> 设置为0表示AI会在目标刚好离开SightRadius时立即失去对其的感知
	AISenseConfig_Sight->LoseSightRadius = 0.f;

	// 设置外围视野角度（单位：度）--> 360.0f表示AI拥有360度全方位视野，没有视觉盲区
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.0f;

	// 创建一个AI感知组件，用于处理所有感知信息（视觉、听觉等）
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");

	// 将上面配置的视觉感知添加到感知组件中 --> ConfigureSense方法将感知配置注册到感知组件
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);

	// 设置主导感知类型：当多个感知同时检测到目标时，主导感知的类型将决定感知更新的主要方式
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	// 绑定感知更新事件：当AI感知到目标状态变化时（发现、丢失等）会触发此事件
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AFightAIController::OnEnemyPerceptionUpdated);

	// 将AI控制器分配到特定的团队ID，这里设置为1，表示该AI属于团队1
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AFightAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AFightAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? 
			ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4:
			CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AFightAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				// 此处的FName("TargetActor")应与行为树黑板中的键名一致
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}
