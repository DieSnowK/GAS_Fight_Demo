// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"


class AEnemyCharacter;


/**
 * @brief 声明动态多播委托：等待生成敌人完成时的回调
 * @param 生成的敌人数组
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AEnemyCharacter*>&, SpawnedEnemies);


/**
 * @brief 等待游戏事件并异步生成敌人
 */
UCLASS()
class GAS_FIGHT_DEMO_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 静态工厂方法, 创建任务实例
	 */
	UFUNCTION(BlueprintCallable, Category = "Fight|AbilityTask",
		meta = (DisplayName = "Wait Gameplay Event And Spawn Enemies",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true",
		NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(UGameplayAbility* OwningAbility,
		FGameplayTag EventTag, TSoftClassPtr<AEnemyCharacter> SoftEnemyClassToSpawn,
		int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius);

	/**
	 * @brief 蓝图可分配委托, 生成完成时触发
	 */
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	/**
	 * @brief 蓝图可分配委托, 生成失败时触发
	 */
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;

	//~ Begin UGameplayTask Interface
	/**
	 * @brief 任务销毁时的回调函数
	 */
	virtual void OnDestroy(bool bInOwnerFinished) override;
	/**
	 * @brief 任务激活时的回调函数
	 */
	virtual void Activate() override;
	//~ End UGameplayTask Interface

private:
	// 缓存的任务参数
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;

	// 委托句柄
	FDelegateHandle DelegateHandle;

	/**
	 * @brief 处理接收到的游戏事件
	 * @param InPayload 游戏事件数据
	 */
	void OnGameplayEventReceived(const FGameplayEventData* InPayload);
	/**
	 * @brief 处理敌人类加载完成的回调
	 */
	void OnEnemyClassLoaded();
};
