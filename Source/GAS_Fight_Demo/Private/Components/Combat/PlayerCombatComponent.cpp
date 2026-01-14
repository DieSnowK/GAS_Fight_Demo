// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/FightPlayerWeapon.h"
#include "GAS/FightGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "GASDebugHelper.h"

AFightPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AFightPlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AFightPlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	return Cast<AFightPlayerWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UPlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// 检查目标是否已经处理过，避免重复处理
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	// 将目标添加到已处理列表中
	OverlappedActors.AddUnique(HitActor);

	// 创建游戏事件数据
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn(); // 设置事件发起者为拥有该组件的Pawn
	Data.Target = HitActor;            // 设置事件目标为被命中的Actor

	// 发送近战命中事件给拥有该组件的Pawn
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),
		FightGameplayTags::Shared_Event_MeleeHit, Data);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),
		FightGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UPlayerCombatComponent::OnWeaponPulledFromTarget(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),
		FightGameplayTags::Player_Event_HitPause, FGameplayEventData());

	// TODO: 在此处添加武器从目标上移开时的处理逻辑
}
