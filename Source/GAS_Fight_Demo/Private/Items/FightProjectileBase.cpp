// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/FightProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FightFunctionLibrary.h"
#include "GAS/FightGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "GASDebugHelper.h"


AFightProjectileBase::AFightProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);

	// 只用于查询
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 设置碰撞响应：阻挡玩家、动态物体和静态物体
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// 绑定碰撞和重叠事件
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 700.f;
	ProjectileMovementComp->MaxSpeed = 900.f;
	ProjectileMovementComp->Velocity = FVector(1.f, 0.f, 0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	// 设置投射物生命周期（4秒后自动销毁）
	InitialLifeSpan = 4.f;
}

void AFightProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 根据伤害策略调整碰撞设置
	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AFightProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 调用蓝图事件生成命中特效
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);

	// 检查碰撞对象是否为Pawn且是否为敌对目标
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn || !UFightFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UFightFunctionLibrary::NativeDoesActorHaveTag(HitPawn, FightGameplayTags::Player_Status_Blocking);
	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UFightFunctionLibrary::IsValidBlock(this, HitPawn);
	}

	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = HitPawn;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn,
			FightGameplayTags::Player_Event_SuccessfulBlock, Data);
	}
	else
	{
		HandleApplyProjectileDamage(HitPawn, Data);
	}

	Destroy();
}

void AFightProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OverlappedActors.Contains(OtherActor))
	{
		return;
	}

	OverlappedActors.AddUnique(OtherActor);

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		FGameplayEventData Data;
		Data.Instigator = GetInstigator();
		Data.Target = HitPawn;

		if (UFightFunctionLibrary::IsTargetPawnHostile(GetInstigator(), HitPawn))
		{
			HandleApplyProjectileDamage(HitPawn, Data);
		}
	}
}

void AFightProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayLoad)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(), 
		TEXT("ProjectileDamageEffectSpecHandle is not valid: %s"), *GetActorNameOrLabel());

	// 应用GameplayEffect到目标Actor
	const bool bWasApplied = UFightFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(
		GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);

	// 如果成功应用伤害，发送受击反应事件
	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitPawn, FightGameplayTags::Shared_Event_HitReact, InPayLoad);
	}
}
