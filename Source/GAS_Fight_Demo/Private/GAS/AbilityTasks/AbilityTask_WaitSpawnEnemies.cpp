// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Characters/EnemyCharacter.h"

#include "GASDebugHelper.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(
    UGameplayAbility* OwningAbility, FGameplayTag EventTag,
    TSoftClassPtr<AEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, 
    const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	// 移除之前注册的委托句柄
	Delegate.Remove(DelegateHandle);

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	// 查找或创建 指定事件标签 的多播委托
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	// 将OnGameplayEventReceived方法注册到委托
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		// 异步加载敌人类资源 -> 加载完成后调用 OnEnemyClassLoaded 方法
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded));
	}
	else
	{
		// 如果软引用为空，触发生成失败委托
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			// 广播空数组
			DidNotSpawn.Broadcast(TArray<AEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	// 获取加载的类引用和世界上下文
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World)
	{
		// 加载失败或世界无效，触发生成失败委托
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AEnemyCharacter*>());
		}

		EndTask();
		return;
	}

	TArray<AEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParam;
	// 设置碰撞处理方式：如果碰撞，尝试调整位置但始终生成
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 循环生成指定数量的敌人
	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{
		// 在导航系统中获取随机可达点
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);

		// 增加垂直偏移 --> 避免敌人卡在地面下
		RandomLocation += FVector(0.f, 0.f, 150.f);

		// 计算生成朝向, 在实际生成时确定, 具体方向通过 [(Montage + BT) --> Motion Warping] 控制
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		// 实际生成敌人Actor
		AEnemyCharacter* SpawnedEnemy = World->SpawnActor<AEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParam);

		// 如果生成成功，添加到数组
		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	// 广播结果委托
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			// 成功生成敌人，广播OnSpawnFinished委托
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			// 没有生成任何敌人，广播DidNotSpawn委托
			DidNotSpawn.Broadcast(TArray<AEnemyCharacter*>());
		}
	}

	EndTask();
}
