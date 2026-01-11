// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/FightGameplayAbility.h"
#include "GAS/FightAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/FightGameplayTags.h"
#include "FightFunctionLibrary.h"


void UFightGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	// 调用父类的OnGiveAbility函数，确保基础功能正常执行
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EFightAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			// TryActivateAbility会检查能力是否可以激活，并在可以激活时执行激活逻辑
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UFightGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 调用父类的EndAbility函数，确保基础功能正常执行 --> 这会处理能力结束的基本逻辑，如清理激活信息等
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 对于OnGiven策略的能力，需要在结束时将其从ASC中清除
	if (AbilityActivationPolicy == EFightAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			// 清除该能力，将其从能力系统中移除 --> ClearAbility会从ASC的能力列表中移除该能力实例
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UFightGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	// 获取角色信息中的Avatar Actor（通常是角色本身），并查找PawnCombatComponent组件
	// FindComponentByClass会在Actor的所有组件中搜索指定类型的组件
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UFightAbilitySystemComponent* UFightGameplayAbility::GetFightAbilitySystemComponentFromActorInfo() const
{
	return Cast<UFightAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UFightGameplayAbility::NativeApplyEffectSpecHandleToTarget(
	AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	// 获取目标角色的能力系统组件
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	// 检查能力和效果规格的有效性
	if (!TargetASC || !InSpecHandle.IsValid())
	{
		return FActiveGameplayEffectHandle();
	}

	// 使用当前能力的ASC将效果应用到目标ASC
	return GetFightAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UFightGameplayAbility::BP_ApplyEffectSpecHandleToTarget(
	AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EFightSuccessType& OutSuccessType)
{
	// 调用原生实现应用效果到目标
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	// 根据句柄是否成功应用设置输出类型
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EFightSuccessType::Successful : EFightSuccessType::Failed;

	// 返回活动的游戏效果句柄
	return ActiveGameplayEffectHandle;
}

void UFightGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(
	const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty())
	{
		return;
	}

	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());

	for (const FHitResult& Hit : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
		{
			if (UFightFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);

				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn, FightGameplayTags::Shared_Event_HitReact, Data);
				}
			}
		}
	}
}
