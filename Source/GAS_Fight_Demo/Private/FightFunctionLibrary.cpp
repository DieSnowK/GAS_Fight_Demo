// Fill out your copyright notice in the Description page of Project Settings.


#include "FightFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/FightAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

UFightAbilitySystemComponent* UFightFunctionLibrary::NativeGetFighterASCFromActor(AActor* InActor)
{
	check(InActor);
	return CastChecked<UFightAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

/**
* 1. 动态生命周期 Loose标签是临时性标签，通过AddLooseGameplayTag添加后，其生命周期由开发者手动管理（通过RemoveLooseGameplayTag显式移除），或通过AddLooseGameplayTagTimed设置自动过期时间。
* 2. 与基标签分离 Loose标签独立于对象的基标签集（Base Tags），后者通常存储在UAbilitySystemComponent的BaseTags成员中，而Loose标签存储在ActiveTags的LooseTags容器里。
* 3. 引用计数机制 Loose标签采用引用计数管理（通过FGameplayTagCount结构），允许多次添加相同标签（引用计数递增），需匹配次数的移除操作才会彻底删除。
*/
void UFightFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UFightAbilitySystemComponent* ASC = NativeGetFighterASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UFightFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UFightAbilitySystemComponent* ASC = NativeGetFighterASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UFightFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UFightAbilitySystemComponent* ASC = NativeGetFighterASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UFightFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EFightConfirmType& OutConfirmResult)
{
	OutConfirmResult = NativeDoesActorHaveTag(InActor, TagToCheck) ? EFightConfirmType::Yes : EFightConfirmType::No;
}

UPawnCombatComponent* UFightFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UFightFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EFightValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EFightValidType::Valid : EFightValidType::Invalid;

	return CombatComponent;
}
