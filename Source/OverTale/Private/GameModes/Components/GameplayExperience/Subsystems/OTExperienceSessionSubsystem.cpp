// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Components/GameplayExperience/Subsystems/OTExperienceSessionSubsystem.h"

#include "Engine/AssetManager.h"

FString UExperienceSessionRequest::GetMapName() const
{
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, /*out*/ MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	else
	{
		return FString();
	}
}

FString UExperienceSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;
	CombinedExtraArgs += TEXT("?listen");

	for (const auto& KVP : ExtraArgs)
	{
    	if (!KVP.Key.IsEmpty())
    	{
    		if (KVP.Value.IsEmpty())
    		{
    			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *KVP.Key);
    		}
    		else
    		{
    			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *KVP.Key, *KVP.Value);
    		}
    	}
	}

	return FString::Printf(TEXT("%s%s"),
		*GetMapName(),
		*CombinedExtraArgs);
	
}

bool UExperienceSessionRequest::ValidateAndLogErrors(FText& OutError) const
{
	//TODO:
	return true;
}

void UOTExperienceSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UOTExperienceSessionSubsystem::Deinitialize()
{
	if (GEngine)
	{
		GEngine->OnTravelFailure().RemoveAll(this);
	}

	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
	Super::Deinitialize();
}

bool UOTExperienceSessionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> ChildClasses;
	GetDerivedClasses(GetClass(), ChildClasses, false);

	// Only create an instance if there is not a game-specific subclass
	return ChildClasses.Num() == 0;
}

UExperienceSessionRequest* UOTExperienceSessionSubsystem::CreateExperienceSessionRequest()
{
	return NewObject<UExperienceSessionRequest>(this);
}

void UOTExperienceSessionSubsystem::HostSession(APlayerController* HostingPlayer, UExperienceSessionRequest* Request)
{
	if (Request == nullptr)
	{
		return;
	}

	GetWorld()->ServerTravel(Request->ConstructTravelURL());
	//TODO: GetWorld()->PrepareMapChange(); find out pros cons and how to use it
}
