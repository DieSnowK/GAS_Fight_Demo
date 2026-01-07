// Fill out your copyright notice in the Description page of Project Settings.


#include "FightFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "GAS/FightAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAS/FightGameplayTags.h"

#include "GASDebugHelper.h"


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

bool UFightFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UFightFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UFightFunctionLibrary::ComputeHitReactDirection(
	AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	// 点积: 计算受害者正前方与受害者指向攻击者方向之间的夹角
	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	// 此处的结果是0~180之间的正值
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	// 叉积: 确定攻击者在受害者的左侧还是右侧 --> 标准右手法则: 左上右下 --> 但是在UE中是左手法则，所以是左下右上
	// 叉积几何意义：A×B的结果向量垂直于A和B构成的平面 --> 在UE坐标系中：Z轴向上，所以叉积的Z分量符号表示左右关系
	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

	if (CrossResult.Z < 0.0f)
	{
		OutAngleDifference *= -1.0f;
	}

	// 受击方向计算
	if (OutAngleDifference >= -45.0f && OutAngleDifference <= 45.0f)
	{
		return FightGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference < 45.f && OutAngleDifference >= -135.0f)
	{
		return FightGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference < -135.0f || OutAngleDifference > 135.0f)
	{
		return FightGameplayTags::Shared_Status_HitReact_Back;
	}
	else if (OutAngleDifference > 45.0f && OutAngleDifference <= 135.0f)
	{
		return FightGameplayTags::Shared_Status_HitReact_Right;
	}

	return FightGameplayTags::Shared_Status_HitReact_Front;
}

bool UFightFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	// cos --> 1.0: 平行, 0.0: 垂直, -1.0: 反向
	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	return DotResult < -0.5f; // 60 | 60
}

bool UFightFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator,
	AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UFightAbilitySystemComponent* SourceASC = NativeGetFighterASCFromActor(InInstigator);
	UFightAbilitySystemComponent* TargetASC = NativeGetFighterASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}
