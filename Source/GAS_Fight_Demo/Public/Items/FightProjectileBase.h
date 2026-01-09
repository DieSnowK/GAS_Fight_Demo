// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "FightProjectileBase.generated.h"


class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
struct FGameplayEventData;

/**
 * @brief 定义投射物伤害应用策略
 */
UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,			// 使用碰撞检测（Block）时应用伤害
	OnBeginOverlap  // 使用重叠检测（Overlap）时应用伤害
};


UCLASS()
class GAS_FIGHT_DEMO_API AFightProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFightProjectileBase();

protected:
	virtual void BeginPlay() override;

	// 碰撞盒组件：用于检测碰撞
	UPROPERTY(VisibleDefaultsonly, BlueprintReadonly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	// Niagara粒子组件：投射物的视觉效果
	UPROPERTY(VisibleDefaultsonly, BlueprintReadonly, Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	// 投射物移动组件：处理物理移动
	UPROPERTY(VisibleDefaultsonly, BlueprintReadonly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComp;

	// 投射物伤害策略：选择使用碰撞检测还是重叠检测
	UPROPERTY(EditDefaultsonly, BlueprintReadonly, Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

	// 投射物伤害效果规格句柄：包含要应用的GameplayEffect信息
	UPROPERTY(BlueprintReadOnly, BlueprintReadonly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

	// 碰撞事件回调函数 --> 当投射物碰撞到其他物体时调用
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 重叠开始事件回调函数 --> 当投射物开始与其他物体重叠时调用
	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief 在蓝图中生成投射物命中特效
	 * 
	 * BlueprintImplementableEvent 表示这个函数应该只在蓝图中实现，不应该在C++中提供实现
	 * 
	 * 这是C++和蓝图协作的常见模式：C++提供基础逻辑，蓝图负责视觉效果
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);

private:
	// 处理投射物伤害应用
	void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayLoad);
};
