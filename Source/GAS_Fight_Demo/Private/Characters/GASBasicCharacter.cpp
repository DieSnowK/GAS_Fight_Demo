// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GASBasicCharacter.h"
#include "GAS/FightAbilitySystemComponent.h"


/**
 * @brief 构造函数实现
 *
 * 设置角色的默认属性值
 * 初始化能力系统组件和属性集组件
 *
 * @details
 * 1. 禁用角色的Tick功能以提高性能
 * 2. 设置角色网格体不接收 decals(贴花)
 * 3. 创建并初始化能力系统组件
 * 4. 创建并初始化属性集组件
 */
AGASBasicCharacter::AGASBasicCharacter()
{
	// 禁用角色的Tick功能以提高性能 --> 对于不需要每帧更新的角色，禁用Tick可以提升游戏性能
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// 设置角色网格体不接收 decals(贴花)--> 通常用于血迹、弹孔等效果，禁用可以提高渲染性能
	GetMesh()->bReceivesDecals = false;

	// 创建默认子对象：能力系统组件
	// CreateDefaultSubobject用于在构造函数中创建组件，确保组件在编辑器和运行时都正确初始化
	FightAbilitySystemComponent = CreateDefaultSubobject<UFightAbilitySystemComponent>(TEXT("FightAbilitySystemComponent"));

	// 创建默认子对象：属性集组件
	// 属性集组件包含角色的所有属性，如生命值、怒气值等
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));

	//MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AGASBasicCharacter::GetAbilitySystemComponent() const
{
	return GetFightAbilitySystemComponent();
}

UPawnCombatComponent* AGASBasicCharacter::GetPawnCombatComponent() const
{
	// 基础角色类未实现具体战斗组件，返回空指针
	// 具体的战斗功能将在派生类中实现
	return nullptr;
}

UPawnUIComponent* AGASBasicCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AGASBasicCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (FightAbilitySystemComponent)
	{
		// 初始化能力Actor信息 --> 第一个参数是拥有能力的Actor，第二个参数是AvatarActor（通常为角色本身）
		FightAbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 确保AttributeSet已正确注册到AbilitySystemComponent
		// 这是解决Attribute.Get()断言失败的关键步骤
		//if (BasicAttributeSet)
		//{
		//	// 将属性集作为子对象添加到能力系统组件中
		//	// 确保属性集在能力系统中正确注册，以便能够应用GameplayEffect
		//	FightAbilitySystemComponent->AddAttributeSetSubobject(BasicAttributeSet);
		//}

		// 确保角色启动数据已分配，如果没有分配则输出详细错误信息
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forget to Assign Start up Data to %s"), *GetName());
	}
}


