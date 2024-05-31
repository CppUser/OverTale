// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OTPlayerState.h"

#include "AbilitySystem/OTAbilitySet.h"
#include "AbilitySystem/Components/OTAbilitySystemComponent.h"
#include "Character/OTPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameModes/OTGameMode.h"
#include "GameModes/Components/GameplayExperience/OTExperienceManager.h"

const FName AOTPlayerState::NAME_OTAbilityReady("OTAbilitiesReady");

AOTPlayerState::AOTPlayerState(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	ASC = ObjInit.CreateDefaultSubobject<UOTAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(false);
}

UOTAbilitySystemComponent* AOTPlayerState::GetOTAbilitySystemComponent() const
{
	return ASC;
}

UAbilitySystemComponent* AOTPlayerState::GetAbilitySystemComponent() const
{
	return GetOTAbilitySystemComponent();
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

	for (const UOTAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(ASC, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_OTAbilityReady);
	
}

void AOTPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(ASC);
	ASC->InitAbilityActorInfo(this,GetPawn());

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
