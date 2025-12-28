// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/GASBasicCharacter.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UPlayerCombatComponent;
class UPlayerUIComponent;

/**
 * @brief 玩家角色类
 *
 * 继承自AGASBasicCharacter，代表游戏中的玩家控制角色（英雄）
 * 实现了第三人称视角控制、输入处理、摄像机系统等功能
 *
 * @details
 * 1. 集成摄像机系统，支持第三人称视角
 * 2. 实现增强输入系统，处理玩家输入
 * 3. 集成英雄战斗组件，处理战斗相关逻辑
 * 4. 支持能力系统输入处理
 *
 * @see AGASBasicCharacter
 */
UCLASS()
class GAS_FIGHT_DEMO_API AMainCharacter : public AGASBasicCharacter
{
	GENERATED_BODY()
	
public:
	AMainCharacter();

	//~ Begin IPawnCombatInterface Interface.
	/**
	 * @brief 获取角色战斗组件
	 *
	 * 实现IPawnCombatInterface接口，返回英雄的战斗组件
	 *
	 * @return 返回英雄的战斗组件指针
	 *
	 * @see UPlayerCombatComponent
	 */
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface.

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	//~ End IPawnUIInterface Interface.

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief 设置玩家输入组件
	 *
	 * 重写父类函数，用于绑定玩家输入动作
	 * 配置增强输入系统和输入映射
	 *
	 * @param PlayerInputComponent 玩家输入组件指针
	 *
	 * @details
	 * 1. 验证输入配置数据资产的有效性
	 * 2. 获取并配置增强输入子系统
	 * 3. 绑定移动、视角和能力输入动作
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

private:
#pragma region Componets

	/**
	 * @brief 摄像机摇臂组件
	 *
	 * 用于实现第三人称视角的摄像机摇臂
	 * 控制摄像机与角色的距离和角度
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/**
	 * @brief 跟随摄像机组件
	 *
	 * 第三人称视角的主摄像机
	 * 附加在摄像机摇臂上，跟随角色移动
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/**
	 * @brief 英雄战斗组件
	 *
	 * 英雄特有的战斗组件，处理英雄战斗相关逻辑
	 * 如武器管理、技能释放等
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCombatComponent> PlayerCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UPlayerUIComponent* PlayerUIComponent;

#pragma endregion


#pragma region Input

	/**
	 * @brief 输入配置数据资产
	 *
	 * 包含输入映射配置的数据资产
	 * 定义了输入动作与游戏行为的映射关系
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	/**
	 * @brief 处理能力输入按下事件
	 *
	 * 处理玩家按下能力相关输入时的逻辑
	 * 通知能力系统组件处理输入按下事件
	 *
	 * @param InInputTag 与按下输入相关联的GameplayTag
	 */
	void Input_AbilityInputPressed(FGameplayTag InInputTag);

	/**
	 * @brief 处理能力输入释放事件
	 *
	 * 处理玩家释放能力相关输入时的逻辑
	 * 通知能力系统组件处理输入释放事件
	 *
	 * @param InInputTag 与释放输入相关联的GameplayTag
	 */
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

public:
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const
	{
		return PlayerCombatComponent;
	}
};
