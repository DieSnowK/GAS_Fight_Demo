// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components//CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"

#include "GASDebugHelper.h"
#include "Game/FightBaseGameMode.h"
#include "GAS/FightGameplayTags.h"
#include "GAS/FightAbilitySystemComponent.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(45.f, 100.f);

	// 禁用Controoler旋转，让角色移动时自动朝向移动方向
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 创建相机杆
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;

	// 设置相机杆的偏移量，调整摄像机相对于角色的位置 --> 实现一种类似越肩视角
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);

	// 相机杆随Controoler旋转
	CameraBoom->bUsePawnControlRotation = true;

	// 创建相机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // SocketName 相当于相机臂的末端

	// 相机本身不需要随着玩家的鼠标一起转动 --> 相机不直接使用Pawn控制旋转，而是通过摇臂间接控制
	FollowCamera->bUsePawnControlRotation = false;

	// 配置角色移动组件
	// 设置角色移动时自动朝向移动方向 --> 设置角色朝向与移动方向一致
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 设置角色旋转速率，仅在Yaw(偏航轴)上旋转（水平方向）
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// 设置最大行走速度
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	// 设置行走时的制动减速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// 创建英雄战斗组件 --> 用于处理英雄特有的战斗逻辑
	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));

	// 创建玩家UI组件
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

UPawnCombatComponent* AMainCharacter::GetPawnCombatComponent() const
{
	return PlayerCombatComponent;
}

UPawnUIComponent* AMainCharacter::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* AMainCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * @brief 设置玩家输入组件实现
 *
 * 配置增强输入系统和输入映射
 *
 * @details
 * 1. 验证输入配置数据资产的有效性
 * 2. 获取并配置增强输入子系统
 * 3. 绑定移动、视角和能力输入动作
 */
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 检查是否分配了有效的输入配置数据资产
	checkf(InputConfigDataAsset, TEXT("Forget to assign a valid data asset as input config"));

	// 获取本地玩家对象，用于访问输入子系统
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	// 获取增强输入子系统 --> ULocalPlayer::GetSubsystem用于获取与本地玩家关联的子系统实例
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	// 验证子系统是否有效
	check(Subsystem);

	// 添加默认的输入映射上下文 --> 输入映射上下文定义了输入动作与处理函数之间的映射关系
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0); // 0表示优先级，数值越小优先级越高

	UPlayerInputComponent* FightInputComponent = CastChecked<UPlayerInputComponent>(PlayerInputComponent);

	// 绑定移动输入动作 --> 使用输入配置数据资产中的配置，将InputTag_Move标签与Input_Move函数绑定 --> 此处调用的接口将两者联系起来
	// ETriggerEvent::Triggered表示触发事件（持续按住）
	FightInputComponent->BindNativeInputAction(InputConfigDataAsset, FightGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Move); // 编译器自动推导泛型参数了
	FightInputComponent->BindNativeInputAction(InputConfigDataAsset, FightGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	FightInputComponent->BindNativeInputAction(InputConfigDataAsset, FightGameplayTags::InputTag_SwitchTarget,
		ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	FightInputComponent->BindNativeInputAction(InputConfigDataAsset, FightGameplayTags::InputTag_SwitchTarget,
		ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	FightInputComponent->BindNativeInputAction(InputConfigDataAsset, FightGameplayTags::InputTag_PickUp_Stones,
		ETriggerEvent::Started, this, &ThisClass::Input_PickUpStonesStarted);

	// 绑定能力输入动作(按下和释放) --> 将能力输入标签与对应的按下和释放处理函数绑定
	// 这允许处理复杂的输入事件，如技能释放
	FightInputComponent->BindAbilityInputAction(InputConfigDataAsset, this,
		&ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (FightAbilitySystemComponent)
	{
		if (!CharacterStartUpData.IsNull())
		{
			// 同步加载启动数据资产 --> LoadSynchronous会阻塞直到资源加载完成，适用于初始化阶段
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			{
				int32 AbilityApplyLevel = 1;

				//if (AWarriorBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AWarriorBaseGameMode>())
				//{
				//	switch (BaseGameMode->GetCurrentGameDifficulty())
				//	{
				//	case EWarriorGameDifficulty::Easy:
				//		AbilityApplyLevel = 4;
				//		break;
				//	case EWarriorGameDifficulty::Normal:
				//		AbilityApplyLevel = 3;
				//		break;
				//	case EWarriorGameDifficulty::Hard:
				//		AbilityApplyLevel = 2;
				//		break;
				//	case EWarriorGameDifficulty::Hell:
				//		AbilityApplyLevel = 1;
				//		break;
				//	}
				//}

				// 将加载的数据应用到能力系统组件
				LoadedData->GiveToAbilitySystemComponent(FightAbilitySystemComponent, AbilityApplyLevel);
			}
		}
	}
}

void AMainCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// 获取二维移动向量(X轴和Y轴) --> FVector2D的X分量表示左右移动，Y分量表示前后移动
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// 获取控制器的Yaw旋转(水平旋转)，用于确定移动方向 --> 仅使用偏航旋转，忽略俯仰和翻滚
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		// 计算向前方向向量 --> RotateVector将向量按照指定旋转进行变换
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		// 添加前后移动输入 --> AddMovementInput是角色移动的基础函数
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	// FVector2D的X分量表示水平旋转，Y分量表示垂直旋转
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AMainCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ? FightGameplayTags::Player_Event_SwitchTarget_Right : FightGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);
}

void AMainCharacter::Input_PickUpStonesStarted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this, FightGameplayTags::Player_Event_ConsumeStones, Data);
}

void AMainCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	// 通知能力系统组件输入被按下 --> OnAbilityInputPressed会查找与输入标签匹配的能力并尝试激活
	FightAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AMainCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	// 通知能力系统组件输入被释放 --> OnAbilityInputReleased会处理能力的释放逻辑
	FightAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
