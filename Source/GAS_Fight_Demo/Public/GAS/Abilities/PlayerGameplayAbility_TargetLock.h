// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/FightPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_TargetLock.generated.h"


class UFightWidgetBase;
class UInputMappingContext;


UCLASS()
class GAS_FIGHT_DEMO_API UPlayerGameplayAbility_TargetLock : public UFightPlayerGameplayAbility
{
	GENERATED_BODY()
	
protected:
	// ~Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~End UGameplayAbility Interface

	/**
	 * @brief 目标锁定每帧更新函数
	 */
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	/**
	 * @brief 切换锁定目标函数
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	/**
	 * brief 尝试锁定目标
	 */
	void TryLockOnTarget();

	/**
	 * @brief 获取可锁定目标Actor列表的方法
	 */
	void GetAvailableActorsToLock();

	/**
	 * @brief 从可锁定目标Actor列表中获取最近的目标Actor
	 */
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	/**
	 * @brief 获取目标Actor左右两侧的可用锁定目标Actor列表
	 */
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);

	/**
	 * @brief 绘制目标锁定UI组件
	 */
	void DrawTargetLockWidget();

	/**
	 * @brief 设置目标锁定UI组件位置
	 */
	void SetTargetLockWidgetPosition();

	/**
	 * @brief 初始化目标锁定时的移动设置
	 */
	void InitTargetLockMovement();

	/**
	 * @brief 初始化目标锁定输入映射上下文
	 */
	void InitTargetLockMappingContext();


	void CancelTargetLockAbility();
	void CleanUp();
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();


	// 盒体追踪的距离
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance{ 5000.f };

	// 盒体追踪的大小
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	// 盒体追踪的碰撞通道 --> 指定要检测哪些类型的对象
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistentDebugShape =  false;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TSubclassOf<UFightWidgetBase> TargetLockWidgetClass;

	// 目标锁定UI组件旋转插值速度
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockRotationInterpSpeed = 5.0f;

	// 目标锁定时玩家最大行走速度
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockMaxWalkSpeed = 150.0f;

	// 目标锁定输入映射上下文
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	UInputMappingContext* TargetLockMappingContext;

	// 目标锁定时相机偏移距离 
	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockCameraOffsetDistance = 15.f;

	// 存储找到的可锁定目标Actor列表
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	// 当前锁定的目标Actor
	UPROPERTY()
	AActor* CurrentLockedActor;

	// 当前绘制的目标锁定UI组件
	UPROPERTY()
	UFightWidgetBase* DrawnTargetLockWidget;

	// 目标锁定UI组件的大小
	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	// 缓存的默认最大行走速度
	UPROPERTY()
	float CachedDefaultMaxWalkSpeed{ 0.5f };
};
