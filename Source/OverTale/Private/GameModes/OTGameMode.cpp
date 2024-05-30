// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTale/Public/GameModes/OTGameMode.h"

#include "Character/OTCharacter.h"
#include "GameModes/OTGameState.h"
#include "GameModes/OTWorldSettings.h"
#include "GameModes/Components/OTExperienceManager.h"
#include "Player/OTPlayerController.h"
#include "System/OTAssetManager.h"
#include "UI/OTHUD.h"

AOTGameMode::AOTGameMode(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	GameStateClass = AOTGameState::StaticClass();
	PlayerControllerClass = AOTPlayerController::StaticClass();
	DefaultPawnClass = AOTCharacter::StaticClass();
	HUDClass = AOTHUD::StaticClass();
}

void AOTGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this,&ThisClass::LoadExperience);
}

void AOTGameMode::LoadExperience()
{

	FPrimaryAssetId ExperienceID;
	FString ExperienceSource;

	//retrieve experience from world settings 
	if (!ExperienceID.IsValid())
	{
		AOTWorldSettings* WorldSettings = Cast<AOTWorldSettings>(GetWorldSettings());

		ExperienceID = WorldSettings->GetDefaultGameplayExperience();
		ExperienceSource = TEXT("WorldSettings");
	}

	UOTAssetManager& AssetManager = UOTAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceID.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceID, /*out*/ Dummy))
	{
		UE_LOG(LogTemp, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"), *ExperienceID.ToString());
		ExperienceID = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	if (!ExperienceID.IsValid())
	{

		//@TODO: Pull this from a config setting or something
		ExperienceID = FPrimaryAssetId(FPrimaryAssetType("OTExperienceDefinition"), FName("B_DefaultExperience"));
		ExperienceSource = TEXT("Default");
	}
	
	OnExperienceLoaded(ExperienceID,ExperienceSource);
}

void AOTGameMode::OnExperienceLoaded(FPrimaryAssetId ExperienceID, const FString& ExperienceSource)
{
	if (ExperienceID.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceID.ToString(), *ExperienceSource);

		UOTExperienceManager* ExperienceManager = GameState->FindComponentByClass<UOTExperienceManager>();
		check(ExperienceManager)
		ExperienceManager->SetCurrentExperience(ExperienceID);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}
