// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/GASPlayerLinkedAnimLayer.h"
#include "AnimInstances/Player/GASPlayerAnimInstance.h"

UGASPlayerAnimInstance* UGASPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UGASPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
