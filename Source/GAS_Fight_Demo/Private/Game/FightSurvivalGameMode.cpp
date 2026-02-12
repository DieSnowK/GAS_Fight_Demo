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

	// 尝试加载保存的游戏难度设置，如果成功则应用到当前游戏模式
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
	// 检查并获取关卡中的所有目标点 --> TargetPoint是UE中的一种标记点，用于指定敌人生成位置
	if (TargetPointArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointArray);
	}
	checkf(!TargetPointArray.IsEmpty(), TEXT("No Target Points found in the level %s for spawning enemies"), *GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;

	// 设置Actor生成参数
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 按TableRow遍历一个波次应生成的Enemy --> 一个波次可能生成多种Enemy，每种Enemy的生成数量也可能不同
	for (const FFightEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;
		}

		// 根据配置的范围随机决定本次生成多少这种敌人
		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		// 从预加载的Map中获取已加载的敌人类 (TSoftClassPtr -> UClass*)
		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		// 根据NumToSpawn循环生成指定数量的敌人
		for (int32 i = 0; i < NumToSpawn; i++)
		{
			// 随机选择一个目标点作为生成原点
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointArray.Num() - 1);
			// 获取目标点的位置
			const FVector SpawnOrigin = TargetPointArray[RandomTargetPointIndex]->GetActorLocation();
			// 获取目标点的朝向作为生成旋转
			const FRotator SpawnRotation = TargetPointArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			// 在生成原点周围寻找导航网格上的随机可达点
			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, SpawnOrigin, RandomLocation, 400.0f);
			// 添加垂直偏移，防止敌人卡在地面下
			RandomLocation += FVector(0.0f, 0.0f, 150.0f);

			// 实际生成敌人Actor
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
