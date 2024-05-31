// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/OTPawnExtComponent.h"
#include "Character/OTPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Utils/OTGameplayTags.h"

const FName UOTPawnExtComponent::NAME_ActorFeatureName("PawnExtension");

UOTPawnExtComponent::UOTPawnExtComponent(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	PawnData = nullptr;
}

bool UOTPawnExtComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == OTGameplayTags::InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == OTGameplayTags::InitState_Spawned && DesiredState == OTGameplayTags::InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}
		
		return true;
	}
	else if (CurrentState == OTGameplayTags::InitState_DataAvailable && DesiredState == OTGameplayTags::InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, OTGameplayTags::InitState_DataAvailable);
	}
	else if (CurrentState == OTGameplayTags::InitState_DataInitialized && DesiredState == OTGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UOTPawnExtComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	if (DesiredState == OTGameplayTags::InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
}

void UOTPawnExtComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == OTGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UOTPawnExtComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = { OTGameplayTags::InitState_Spawned, OTGameplayTags::InitState_DataAvailable, OTGameplayTags::InitState_DataInitialized, OTGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UOTPawnExtComponent::SetPawnData(const UOTPawnData* InPawnData)
{
	check(InPawnData);

	/////////////////////////////////////////////////////////////
	APawn* Pawn = GetPawnChecked<APawn>();
	
	//TODO: Check if properly doing condition check
	if (PawnData)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}
	////////////////////////////////////////////////////////////////
	
	PawnData = InPawnData;

	CheckDefaultInitialization();
}

void UOTPawnExtComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void UOTPawnExtComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UOTPawnExtComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("UOTPawnExtComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(UOTPawnExtComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one UOTPawnExtComponent should exist on [%s]."), *GetNameSafe(GetOwner()));
	
	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void UOTPawnExtComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// Notifies state manager that we have spawned, then try rest of default initialization
	ensure(TryToChangeInitState(OTGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UOTPawnExtComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}
