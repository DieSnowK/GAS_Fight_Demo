// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/FightPlayerGameplayAbility.h"
#include "PlayerGA_PickUpStones.generated.h"


class AFightStoneBase;


/**
 * @brief 玩家捡起石头能力
 */
UCLASS()
class GAS_FIGHT_DEMO_API UPlayerGA_PickUpStones : public UFightPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	// ~Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~End UGameplayAbility Interface

	UFUNCTION(BlueprintCallable)
	void CollectStones();

	UFUNCTION(BlueprintCallable)
	void ConsumeStones();

private:
	// 盒体追踪距离
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f;

	// 盒体追踪大小
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);

	// 石头追踪通道：指定要检测哪些类型的对象
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> StoneTraceChannel;

	// 是否绘制调试形状（开发时可视化追踪范围）
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugShape = false;

	// 已收集的石头数组
	UPROPERTY()
	TArray<AFightStoneBase*> CollectedStones;
};
