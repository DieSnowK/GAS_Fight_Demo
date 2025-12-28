// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/FightPlayerWeapon.h"

void AFightPlayerWeapon::AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AFightPlayerWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
