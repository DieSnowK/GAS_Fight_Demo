// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"


AMainPlayerController::AMainPlayerController()
{
	PlayerTeamId = FGenericTeamId(0);
}

FGenericTeamId AMainPlayerController::GetGenericTeamId() const
{
	return PlayerTeamId;
}
