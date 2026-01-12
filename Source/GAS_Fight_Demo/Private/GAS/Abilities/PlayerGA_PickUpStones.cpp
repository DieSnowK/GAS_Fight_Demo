// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/PlayerGA_PickUpStones.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/PickUps/FightStoneBase.h"
#include "Characters/MainCharacter.h"
#include "Components/UI/PlayerUIComponent.h"


void UPlayerGA_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GetPlayerUIComponentFromActorInfo()->OnStoneInteraction.Broadcast(true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayerGA_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetPlayerUIComponentFromActorInfo()->OnStoneInteraction.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayerGA_PickUpStones::CollectStones()
{
	CollectedStones.Empty();

	AMainCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	TArray<FHitResult> TraceHits;

	// 执行盒体多重追踪（检测玩家脚下区域的石头）
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		PlayerCharacter, // WorldContextObject - 世界上下文对象（玩家角色）
		PlayerCharacter->GetActorLocation(), // 开始位置 - 玩家当前位置
		PlayerCharacter->GetActorLocation() + -PlayerCharacter->GetActorUpVector() * BoxTraceDistance, // 结束位置 - 玩家位置向下延伸BoxTraceDistance距离
		TraceBoxSize / 2.f, // 盒体半尺寸（BoxTrace需要半尺寸）
		(-PlayerCharacter->GetActorUpVector()).ToOrientationRotator(), // 盒体旋转 - 使用向下的方向作为朝向
		StoneTraceChannel, // 要检测的碰撞通道 - 在编辑器中配置
		false, // 是否忽略自身
		TArray<AActor*>(), // 要忽略的Actor列表
		bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits, // 输出参数 - 存储所有命中结果
		true // 是否忽略追踪方向 - 进行盒体扫描而非射线追踪
	);

	// 遍历所有命中结果，收集石头
	for (const FHitResult& TraceHit : TraceHits)
	{
		if (AFightStoneBase* FoundStone = Cast<AFightStoneBase>(TraceHit.GetActor()))
		{
			CollectedStones.AddUnique(FoundStone);
		}
	}

	// 如果没有收集到任何石头，取消能力
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UPlayerGA_PickUpStones::ConsumeStones()
{
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	for (AFightStoneBase* CollectedStone : CollectedStones)
	{
		if (CollectedStone)
		{
			CollectedStone->Consume(GetFightAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
		}
	}
}
