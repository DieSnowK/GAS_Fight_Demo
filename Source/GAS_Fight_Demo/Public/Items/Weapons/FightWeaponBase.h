// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FightWeaponBase.generated.h"


class UBoxComponent;


/**
 * @brief 目标交互委托声明
 *
 * 声明一个带有一个AActor*参数的委托
 * 用于在武器与目标发生交互时通知监听者
 *
 * @param AActor* 与武器发生交互的目标演员
 */
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)


/**
 * @brief 基础武器类
 *
 * 继承自AActor，作为所有武器的基类
 * 提供武器的基本功能，包括网格体、碰撞检测和交互事件
 *
 * @details
 * 1. 包含武器静态网格体组件
 * 2. 包含武器碰撞盒组件
 * 3. 处理碰撞事件
 * 4. 提供武器与目标交互的委托
 *
 * @see AActor
 */
UCLASS()
class GAS_FIGHT_DEMO_API AFightWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * @brief 构造函数
	 *
	 * 设置武器Actor的默认属性值
	 * 初始化武器网格体和碰撞盒组件
	 */
	AFightWeaponBase();

	/**
	 * @brief 武器击中目标委托
	 *
	 * 当武器击中目标时触发的委托
	 * 可以绑定多个处理函数来响应击中事件
	 */
	FOnTargetInteractedDelegate OnWeaponHitTarget;

	/**
	 * @brief 武器从目标拔出委托
	 *
	 * 当武器从目标拔出时触发的委托
	 * 可以绑定多个处理函数来响应拔出事件
	 */
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;

protected:
	/**
	 * @brief 武器网格体组件
	 *
	 * 武器的可视化网格体组件
	 * 作为武器的根组件
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	/**
	 * @brief 武器碰撞盒组件
	 *
	 * 用于检测武器与其它物体的碰撞
	 * 默认不启用碰撞检测，需要时手动启用
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	/**
	 * @brief 碰撞盒开始重叠事件处理函数
	 *
	 * 当武器碰撞盒开始与其它组件重叠时调用
	 * 检查重叠对象是否为有效目标，如果是则触发击中事件
	 *
	 * @param OverlappedComponent 发生重叠的组件（武器碰撞盒）
	 * @param OtherActor 与之重叠的其它演员
	 * @param OtherComp 与之重叠的其它组件
	 * @param OtherBodyIndex 其它物理体的索引
	 * @param bFromSweep 是否为扫掠结果
	 * @param SweepResult 扫掠结果信息
	 *
	 * @details
	 * 1. 获取武器的拥有者（Pawn）
	 * 2. 检查重叠对象是否为Pawn
	 * 3. 确保重叠对象不是武器拥有者本身
	 * 4. 触发武器击中目标事件
	 */
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief 碰撞盒结束重叠事件处理函数
	 *
	 * 当武器碰撞盒结束与其它组件重叠时调用
	 * 检查重叠对象是否为有效目标，如果是则触发拔出事件
	 *
	 * @param OverlappedComponent 结束重叠的组件（武器碰撞盒）
	 * @param OtherActor 与之结束重叠的其它演员
	 * @param OtherComp 与之结束重叠的其它组件
	 * @param OtherBodyIndex 其它物理体的索引
	 *
	 * @details
	 * 1. 获取武器的拥有者（Pawn）
	 * 2. 检查重叠对象是否为Pawn
	 * 3. 确保重叠对象不是武器拥有者本身
	 * 4. 触发武器从目标拔出事件
	 */
	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const
	{
		return WeaponCollisionBox;
	}
};
