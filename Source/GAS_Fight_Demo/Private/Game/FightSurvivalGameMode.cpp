// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FightSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "FightFunctionLibrary.h"

#include "GASDebugHelper.h"


void AFightSurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	EFightGameDifficulty SavedGameDifficulty;

	if (UFightFunctionLibrary::TryLoadSavedGameDifficulty(SavedGameDifficulty))
	{
		CurrentGameDifficulty = SavedGameDifficulty;
	}
}

void AFightSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assign a valid data table in survival game mode blueprint"));

	SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreloadNextWaveEnemies();
}

void AFightSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentSurvivalGameModeState == EFightSurvivalGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.0f;

			SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState::SpawningNewWave);
		}
	}

	if (CurrentSurvivalGameModeState == EFightSurvivalGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceStart = 0.0f;

			SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState::InProgress);
		}
	}

	if (CurrentSurvivalGameModeState == EFightSurvivalGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaTime;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.0f;

			CurrentWaveCount++;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState::WaitSpawnNewWave);
				PreloadNextWaveEnemies();
			}
		}
	}
}

void AFightSurvivalGameMode::SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState InState)
{
	CurrentSurvivalGameModeState = InState;

	OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AFightSurvivalGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWavesToSpawn;
}

void AFightSurvivalGameMode::PreloadNextWaveEnemies()
{
	if (HasFinishedAllWaves())
	{
		return;
	}

	PreLoadedEnemyClassMap.Empty();

	for (const FFightEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, LoadedEnemyClass);
					}
				}
			)
		);
	}
}

FFightEnemyWaveSpawnerTableRow* AFightSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

	FFightEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FFightEnemyWaveSpawnerTableRow>(RowName, FString());

	checkf(FoundRow, TEXT("Could not find row %s in EnemyWaveSpawnerDataTable"), *RowName.ToString());

	return FoundRow;
}

int32 AFightSurvivalGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointArray);
	}
	checkf(!TargetPointArray.IsEmpty(), TEXT("No Target Points found in the level %s for spawning enemies"), *GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FFightEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;

			UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, SpawnOrigin, RandomLocation, 400.0f);

			RandomLocation += FVector(0.0f, 0.0f, 150.0f);

			AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParam);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}

			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}

	return EnemiesSpawnedThisTime;
}

bool AFightSurvivalGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AFightSurvivalGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;

	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}

	else if (CurrentSpawnedEnemiesCounter <= 0)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;

		SetCurrentSurvivalGameModeState(EFightSurvivalGameModeState::WaveCompleted);
	}
}

void AFightSurvivalGameMode::RegisterSpawnedEnemies(const TArray<AEnemyCharacter*>& InEnemyToRegister)
{
	for (AEnemyCharacter* SpawnedEnemy : InEnemyToRegister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;

			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &AFightSurvivalGameMode::OnEnemyDestroyed);
		}
	}
}
