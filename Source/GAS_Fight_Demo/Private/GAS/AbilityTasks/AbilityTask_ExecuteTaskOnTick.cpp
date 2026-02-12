// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"

UAbilityTask_ExecuteTaskOnTick::UAbilityTask_ExecuteTaskOnTick()
{
	bTickingTask = true;
}

UAbilityTask_ExecuteTaskOnTick* UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	// 使用NewAbilityTask模板函数创建任务实例 --> UAbilityTask提供的工厂函数，用于安全地创建任务实例
	// 它负责：分配内存, 设置Owner Ability, 初始化基础属性
	UAbilityTask_ExecuteTaskOnTick* Node = NewAbilityTask<UAbilityTask_ExecuteTaskOnTick>(OwningAbility);

	return Node;
}

void UAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}