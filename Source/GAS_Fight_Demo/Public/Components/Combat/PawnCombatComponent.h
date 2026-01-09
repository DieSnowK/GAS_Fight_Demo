// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"


class AFightWeaponBase;


/**
 * @brief 切换伤害类型枚举
 *
 * 定义在切换武器碰撞时应该影响哪些武器或身体部位
 * 使用UENUM宏标记，支持蓝图集成和网络复制
 *
 * @see UENUM
 */
UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	//CurrentEquippedWeapon UMETA(DisplayName = "Current Equipped Weapon"),
	//LeftHand UMETA(DisplayName = "Left Hand"),
	//RightHand UMETA(DisplayName = "Right Hand")
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};


/**
 * @brief Pawn战斗组件类
 *
 * 管理角色的武器和战斗相关功能
 * 继承自UPawnExtensionComponentBase，用于扩展Pawn的战斗能力
 *
 * @details
 * 1. 管理角色携带的武器映射
 * 2. 处理武器的注册和查找
 * 3. 控制武器碰撞检测的启用状态
 * 4. 处理武器与目标的交互事件
 *
 * @see UPawnExtensionComponentBase
 */
UCLASS()
class GAS_FIGHT_DEMO_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 注册已生成的武器到角色武器映射中
	 *
	 * 将新生成的武器实例与Gameplay标签关联并存储在武器映射中
	 * 可选择性地将该武器设置为当前装备的武器
	 *
	 * @param InWeaponTagToRegister 用于标识武器的Gameplay标签
	 * @param InWeaponToRegister 需要注册的武器实例指针
	 * @param bRegisterAsEquippedWeapon 是否将该武器注册为当前装备的武器，默认为false
	 *
	 * @details
	 * 1. 检查武器标签是否已存在，避免重复注册
	 * 2. 验证武器实例指针的有效性
	 * 3. 将武器添加到角色携带的武器映射表中
	 * 4. 绑定武器的事件处理函数
	 * 5. 根据需要设置为当前装备的武器
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, 
		AFightWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	/**
	 * @brief 根据Gameplay标签获取角色携带的武器
	 *
	 * 在角色携带的武器映射表中查找指定标签对应的武器实例
	 *
	 * @param InWeaponTagToGet 用于查找武器的Gameplay标签
	 * @return 返回找到的武器实例指针，如果未找到则返回nullptr
	 *
	 * @details
	 * 1. 使用TMap::Find方法在武器映射表中查找
	 * 2. 如果找到则返回武器实例指针，否则返回nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	AFightWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	/**
	 * @brief 当前装备武器的Gameplay标签
	 *
	 * 用于标识角色当前正在使用的武器
	 * 可在蓝图中读取和修改
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	/**
	 * @brief 获取角色当前装备的武器实例
	 *
	 * 根据当前装备武器标签获取对应的武器实例
	 *
	 * @return 返回当前装备的武器实例指针，如果未装备则返回nullptr
	 *
	 * @details
	 * 1. 验证当前装备武器标签的有效性
	 * 2. 调用GetCharacterCarriedWeaponByTag获取武器实例
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	AFightWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	/**
	 * @brief 切换武器碰撞检测的启用状态
	 *
	 * 启用或禁用指定武器的碰撞检测功能
	 *
	 * @param bShouldEnable 是否启用武器碰撞检测
	 * @param ToggleDamageType 指定要切换碰撞检测的武器或身体部位类型，默认为当前装备武器
	 *
	 * @details
	 * 1. 根据ToggleDamageType类型确定要操作的武器
	 * 2. 设置武器碰撞盒的启用状态
	 * 3. 在禁用时清空重叠演员列表
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	/**
	 * @brief 武器击中目标演员时的回调函数
	 *
	 * 当武器击中目标时调用，用于处理击中逻辑
	 *
	 * @param HitActor 被击中的目标演员
	 *
	 * @details
	 * 该函数为虚函数，可在派生类中重写以实现具体逻辑
	 */
	virtual void OnHitTargetActor(AActor* HitActor);

	/**
	 * @brief 武器从目标中拔出时的回调函数
	 *
	 * 当武器从目标中拔出时调用，用于处理拔出逻辑
	 *
	 * @param InteractedActor 交互的目标演员
	 *
	 * @details
	 * 该函数为虚函数，可在派生类中重写以实现具体逻辑
	 */
	virtual void OnWeaponPulledFromTarget(AActor* InteractedActor);

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);

	/**
	 * @brief 切换身体部位碰撞盒的碰撞状态
	 * 
	 * 不在PawnCombatComponent中实现, 在具体的子类中进行实现
	 */
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	/**
	 * @brief 重叠的演员列表
	 *
	 * 存储与武器碰撞盒重叠的演员列表
	 * 用于避免重复处理同一演员的碰撞事件
	 */
	TArray<AActor*> OverlappedActors;

private:
	/**
	 * @brief 角色携带的武器映射表
	 *
	 * 使用Gameplay标签作为键，武器实例作为值进行存储
	 * 用于快速查找和管理角色携带的各种武器
	 */
	TMap<FGameplayTag, AFightWeaponBase*> CharacterCarriedWeaponMap;
};
