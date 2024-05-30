// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/OTPawnExtComponent.h"
#include "Utils/OTGameplayTags.h"

const FName UOTPawnExtComponent::NAME_ActorFeatureName("PawnExtension");

UOTPawnExtComponent::UOTPawnExtComponent(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
}

bool UOTPawnExtComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UOTPawnExtComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UOTPawnExtComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

void UOTPawnExtComponent::CheckDefaultInitialization()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitialization();
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
