// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OTPlayerState.h"
#include "Character/OTPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameModes/OTGameMode.h"
#include "GameModes/Components/OTExperienceManager.h"

AOTPlayerState::AOTPlayerState(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
}

void AOTPlayerState::SetPawnData(const UOTPawnData* InPawnData)
{
	check(InPawnData);
	
	if (PawnData)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}
	
	PawnData = InPawnData;
}

void AOTPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		UOTExperienceManager* ExperienceManager = GameState->FindComponentByClass<UOTExperienceManager>();
		check(ExperienceManager)
		ExperienceManager->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this,&ThisClass::OnExperienceLoaded));
	}
}

void AOTPlayerState::OnExperienceLoaded(const UOTExperienceDefinition* CurrentExperience)
{
	if (AOTGameMode* GM = GetWorld()->GetAuthGameMode<AOTGameMode>())
	{
		if (const UOTPawnData* NewPawnData = GM->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AOTPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}
