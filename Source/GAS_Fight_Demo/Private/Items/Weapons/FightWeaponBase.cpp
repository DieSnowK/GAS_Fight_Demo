// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/FightWeaponBase.h"
#include "Components/BoxComponent.h"
#include "FightFunctionLibrary.h"

#include "GASDebugHelper.h"

/**
 * @brief 构造函数实现
 *
 * 设置武器Actor的默认属性值
 * 初始化武器网格体和碰撞盒组件
 * 配置碰撞事件处理
 *
 * @details
 * 1. 禁用Actor的Tick功能以提高性能
 * 2. 创建并设置武器网格体组件为根组件
 * 3. 创建并配置武器碰撞盒组件
 * 4. 注册碰撞事件处理函数
 */
AFightWeaponBase::AFightWeaponBase()
{
	// 禁用Actor的Tick功能以提高性能 --> 对于不需要每帧更新的武器Actor，禁用Tick可以提升游戏性能
	PrimaryActorTick.bCanEverTick = false;

	// 创建武器网格体组件
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	// 设置武器网格体为根组件 --> SetRootComponent设置Actor的根组件，所有其它组件都将相对于此组件定位
	SetRootComponent(WeaponMesh);

	// 创建武器碰撞盒组件
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	// 将碰撞盒组件附加到根组件 --> SetupAttachment用于将组件附加到另一个组件上
	WeaponCollisionBox->SetupAttachment(GetRootComponent());

	// 设置碰撞盒的大小 --> SetBoxExtent设置盒形碰撞体的大小，参数为各方向的半长
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));

	// 默认禁用碰撞检测 --> ECollisionEnabled::NoCollision表示不进行碰撞检测
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 注册碰撞开始重叠事件处理函数 --> AddUniqueDynamic用于动态绑定UFUNCTION标记的函数到委托
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);

	// 注册碰撞结束重叠事件处理函数
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void AFightWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 获取武器的拥有者Pawn --> GetInstigator用于获取导致此Actor生成的Pawn
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("Weapon Owning Pawn is null"));

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		//if (UFightFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		//{
			//OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		//}

		if (HitPawn != WeaponOwningPawn)
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
#pragma region Debug
#if DEBUG
			Debug::Print(GetName() + TEXT(" begin overlap with ") + HitPawn->GetName(), FColor::Green);
#endif
#pragma endregion
		}
	}
}

void AFightWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("Weapon Owning Pawn is null"));

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		//if (UFightFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		//{
			/*OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);*/
		//}

		if (HitPawn != WeaponOwningPawn)
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
#pragma region Debug
#if DEBUG
			Debug::Print(GetName() + TEXT(" end overlap with ") + HitPawn->GetName());
#endif
#pragma endregion
		}
	}

	// TODO：实现对敌人角色的命中检查
}
