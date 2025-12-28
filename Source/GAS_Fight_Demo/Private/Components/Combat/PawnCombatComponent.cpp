// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/FightWeaponBase.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/FightWeaponBase.h"

#include "GASDebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, 
	AFightWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), 
		TEXT("A name named %s has been already added as a carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	// 将武器标签和武器实例添加到角色携带的武器映射表中
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// 绑定武器的击中目标事件到本组件的OnHitTargetActor处理函数 --> BindUObject用于将 UObject 的成员函数绑定到委托
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	// 绑定武器的从目标拔出事件到本组件的OnWeaponPulledFromTarget处理函数
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTarget);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

#pragma region Debug
#if DEBUG
	const FString WeaponsString = FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s"), 
									*InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	Debug::Print(WeaponsString);
#endif
#pragma endregion
}

AFightWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		// 使用TMap::Find方法在武器映射表中查找指定标签对应的武器 --> Find返回指向值的指针，如果未找到则返回nullptr
		if (AFightWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

AFightWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	// 检查当前装备武器标签是否有效 --> IsValid是FGameplayTag的方法，用于检查标签是否有效
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else
	{
		ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// TODO: 实现击中目标的具体逻辑
	// 可能包括：
	// 1. 应用伤害效果
	// 2. 播放击中特效
	// 3. 触发音效
	// 4. 更新UI信息等
}

void UPawnCombatComponent::OnWeaponPulledFromTarget(AActor* InteractedActor)
{
	// TODO: 实现武器拔出的具体逻辑
	// 可能包括：
	// 1. 处理拔出时的伤害或效果
	// 2. 播放拔出特效
	// 3. 触发音效
	// 4. 更新相关状态等
}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	// 获取当前装备的武器实例
	AFightWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

	check(WeaponToToggle);

	if (bShouldEnable)
	{
		// 启用碰撞检测，设置为QueryOnly模式（仅查询，不产生物理反应）
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
#pragma region Debug
#if DEBUG
		Debug::Print(WeaponToToggle->GetName() + TEXT(" weapon collision enabled."), FColor::Green);
#endif
#pragma endregion
	}
	else
	{
		// 禁用碰撞检测
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 清空重叠演员列表，为下一次启用碰撞做准备
		OverlappedActors.Empty();

#pragma region Debug
#if DEBUG
		Debug::Print(WeaponToToggle->GetName() + TEXT(" weapon collision disabled."));
#endif
#pragma endregion
	}
}

void UPawnCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{

}
