// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Engine/AssetManager.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/FightWidgetBase.h"

#include "GASDebugHelper.h"

AEnemyCharacter::AEnemyCharacter()
{
	// 设置角色自动被AI控制器控制 --> PlacedInWorldOrSpawned表示在世界中放置或生成时自动被AI控制
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 默认禁用控制器旋转，避免角色随控制器旋转而旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 配置角色移动组件参数
	// 让角色的移动组件不再根据控制器的期望旋转来旋转角色
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	// 启用面向移动方向，使角色自动朝向移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 设置旋转速率，仅在偏航轴上旋转（水平方向）
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	// 设置最大行走速度 --> 默认值，可以在DataAsset_EnemyStartUpData中覆盖
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	// 设置制动减速度
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;

	// 创建默认子对象：敌人战斗组件
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");

 	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
 
 	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
 	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
 
 	//LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
 	//LeftHandCollisionBox->SetupAttachment(GetMesh());
 	//LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 	//LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
 
 	//RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
 	//RightHandCollisionBox->SetupAttachment(GetMesh());
 	//RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 	//RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* AEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 手动调用函数, 初始化敌人血量UI小部件
	if (UFightWidgetBase* HealthWidget = Cast<UFightWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreatedWidget(this);
	}
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitEnemyStartUpData();
}

void AEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	int32 AbilityApplyLevel = 1;

	//if (AWarriorBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AWarriorBaseGameMode>())
	//{
	//	switch (BaseGameMode->GetCurrentGameDifficulty())
	//	{
	//	case EWarriorGameDifficulty::Easy:
	//		AbilityApplyLevel = 1;
	//		break;
	//	case EWarriorGameDifficulty::Normal:
	//		AbilityApplyLevel = 2;
	//		break;
	//	case EWarriorGameDifficulty::Hard:
	//		AbilityApplyLevel = 3;
	//		break;
	//	case EWarriorGameDifficulty::Hell:
	//		AbilityApplyLevel = 4;
	//		break;
	//	}
	//}

	// 使用Unreal的资源管理器异步加载 CharacterStartUpData 指定的资源（通常是 DataAsset）--> 异步加载避免阻塞游戏主线程，提高性能
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		// 获取软引用资源的路径，用于异步加载
		CharacterStartUpData.ToSoftObjectPath(),

		// 创建一个异步加载完成时的 Lambda 回调函数，等资源加载完成后自动调用
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]()
			{
				// 在异步加载完成后，获取并使用实际的数据资产对象指针并调用数据资产方法
				// 如果没有获取到数据资产对象指针，则说明加载失败或数据资产未设置
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					// 调用数据资产的方法，把能力（如技能、属性等）赋予当前角色的能力系统组件
					// GiveToAbilitySystemComponent是数据资产中的方法，用于初始化角色的能力系统
					LoadedData->GiveToAbilitySystemComponent(FightAbilitySystemComponent, AbilityApplyLevel);

#pragma region
#if DEBUG
					 Debug::Print(TEXT("Enemy StartUpData loaded and applied successfully!"), FColor::Green, 5.0f);
#endif
#pragma endregion
				}
			}
		)
	);
}
