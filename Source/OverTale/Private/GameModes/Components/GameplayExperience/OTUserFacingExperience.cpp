// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Components/GameplayExperience/OTUserFacingExperience.h"

#include "GameModes/Components/GameplayExperience/Subsystems/OTExperienceSessionSubsystem.h"

UExperienceSessionRequest* UOTUserFacingExperience::CreateRequest(const UObject* WorldContextObject) const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	const FString UserFacingExperienceName = GetPrimaryAssetId().PrimaryAssetName.ToString();

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UExperienceSessionRequest* Result = nullptr;

	if (UOTExperienceSessionSubsystem* Subsystem = GameInstance ? GameInstance->GetSubsystem<UOTExperienceSessionSubsystem>() : nullptr)
	{
		Result = Subsystem->CreateExperienceSessionRequest();
	}

	if (!Result)
	{
		// Couldn't use the subsystem so create one
		Result = NewObject<UExperienceSessionRequest>();
	}
	Result->MapID = MapID;
	Result->ExtraArgs = ExtraArgs;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
