// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GASBasicCharacter.h"
#include "EnemyCharacter.generated.h"


class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;


/**
 * @brief 敌人角色类
 *
 * 继承自AGASBasicCharacter，代表游戏中的敌人角色
 * 实现了敌人特有的功能，如AI控制、敌人战斗组件等
 *
 * @details
 * 1. 自动被AI控制器控制
 * 2. 集成敌人战斗组件
 * 3. 支持通过数据资产进行初始化配置
 *
 * @see AGASBasicCharacter
 */
UCLASS()
class GAS_FIGHT_DEMO_API AEnemyCharacter : public AGASBasicCharacter
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 构造函数
	 *
	 * 设置敌人角色的默认属性值
	 * 配置AI控制和移动相关参数
	 * 初始化敌人战斗组件
	 */
	AEnemyCharacter();

	//~ Begin IPawnCombatInterface Interface.
	/**
	 * @brief 获取角色战斗组件
	 *
	 * 实现IPawnCombatInterface接口，返回敌人的战斗组件
	 *
	 * @return 返回敌人的战斗组件指针
	 *
	 * @see UEnemyCombatComponent
	 */
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface.

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End IPawnUIInterface Interface.

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief 角色被控制器占有时的回调函数
	 *
	 * 当角色被AI控制器占有时调用
	 * 用于初始化敌人启动数据
	 *
	 * @param NewController 占有角色的新控制器（通常为AI控制器）
	 *
	 * @details
	 * 1. 调用父类的PossessedBy函数
	 * 2. 初始化敌人启动数据
	 */
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

	/**
	 * @brief 敌人战斗组件
	 *
	 * 敌人特有的战斗组件，处理敌人战斗相关逻辑
	 * 如武器管理、伤害处理等
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

private:
	/**
	 * @brief 初始化敌人启动数据
	 *
	 * 异步加载并应用敌人的启动数据资产
	 * 包括能力、属性等初始化配置
	 *
	 * @details
	 * 1. 检查启动数据是否有效
	 * 2. 使用资源管理器异步加载数据资产
	 * 3. 加载完成后应用数据到能力系统组件
	 */
	void InitEnemyStartUpData();

public:
	/**
	 * @brief 获取敌人战斗组件
	 *
	 * 内联函数，用于快速获取敌人战斗组件
	 *
	 * @return 返回敌人战斗组件指针
	 */
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const
	{
		return EnemyCombatComponent;
	}
};
