// Fill out your copyright notice in the Description page of Project Settings.


#include "FightGameInstance.h"
#include "MoviePlayer.h"


bool FFightGameLevelSet::IsValid() const
{
	return LevelTag.IsValid() && !Level.IsNull();
}

void UFightGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}

void UFightGameInstance::OnPreLoadMap(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = .5f;
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UFightGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	GetMoviePlayer()->StopMovie();
}

TSoftObjectPtr<UWorld> UFightGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FFightGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid())
		{
			continue;
		}

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
