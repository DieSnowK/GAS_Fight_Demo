// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BasicAttributeSet.h"
#include "FightFunctionLibrary.h"
#include "GAS/FightGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"

#include "GASDebugHelper.h"


UBasicAttributeSet::UBasicAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	InitDamageTaken(0.f);
}

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), 
		*Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("%s's PawnUIComponent is nullptr"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	// 检查被修改的属性是否为当前生命值属性
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		// 数值钳制
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		// 设置修正后的生命值
		SetCurrentHealth(NewCurrentHealth);

		// 通知UI组件当前生命值变化
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	// 检查被修改的属性是否为当前怒气值属性
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UFightFunctionLibrary::AddGameplayTagToActorIfNone(
				Data.Target.GetAvatarActor(), FightGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() == 0.f)
		{
			UFightFunctionLibrary::AddGameplayTagToActorIfNone(
				Data.Target.GetAvatarActor(), FightGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UFightFunctionLibrary::RemoveGameplayTagFromActorIfFound(
				Data.Target.GetAvatarActor(), FightGameplayTags::Player_Status_Rage_Full);
			UFightFunctionLibrary::RemoveGameplayTagFromActorIfFound(
				Data.Target.GetAvatarActor(), FightGameplayTags::Player_Status_Rage_None);
		}

		if (UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	// 检查被修改的属性是否为承受伤害值属性
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrenHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrenHealth);

		// 通知UI组件当前生命值变化
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		 const FString Msg = FString::Printf(TEXT("OldHealth: %f, DamageDone: %f, NewCurrentHealth: %f"),
		 	OldHealth, DamageDone, NewCurrenHealth);
		 Debug::Print(Msg, FColor::Blue);
		 //Debug::Print(TEXT("Current Health Rate: "), (GetCurrentHealth() / GetMaxHealth()));

		// 检查角色是否死亡（生命值为0）
		if (GetCurrentHealth() <= 0.f)
		{
			// 调试：检查 AnimInstance 是否有效
			// if (ACharacter* Character = Cast<ACharacter>(Data.Target.GetAvatarActor()))
			// {
			// 	USkeletalMeshComponent* Mesh = Character->GetMesh();
			// 	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;
			// 	
			// 	UE_LOG(LogTemp, Warning, TEXT("[Death Check] Mesh Valid: %s, AnimInstance Valid: %s"),
			// 		Mesh ? TEXT("YES") : TEXT("NO"),
			// 		AnimInstance ? TEXT("YES") : TEXT("NO"));
			// }
			//
			// UE_LOG(LogTemp, Error, TEXT("[Death] Adding Death Tag to Actor: %s"), *Data.Target.GetAvatarActor()->GetName());

			UFightFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),
				FightGameplayTags::Shared_Status_Dead);

			// UE_LOG(LogTemp, Error, TEXT("[Death] Death Tag Added Successfully"));


		}
	}
}
