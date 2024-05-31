// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/OTHeroComponent.h"
#include "Character/Components/OTPawnExtComponent.h"
#include "Misc/UObjectToken.h"
#include "Utils/OTGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Components/OTAbilitySystemComponent.h"
#include "Character/OTPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Input/OTInputComponent.h"
#include "Input/OTInputConfig.h"
#include "Player/OTLocalPlayer.h"
#include "Player/OTPlayerController.h"
#include "Player/OTPlayerState.h"
#include "UserSettings/EnhancedInputUserSettings.h"


const FName UOTHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UOTHeroComponent::NAME_ActorFeatureName("Hero");


UOTHeroComponent::UOTHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

void UOTHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("[UOTHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("UOTHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("UOTHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UOTHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == OTGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == OTGameplayTags::InitState_Spawned && DesiredState == OTGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AOTPlayerState>())
		{
			return false;
		}
		
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AOTPlayerController* PC = GetController<AOTPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !PC || !PC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == OTGameplayTags::InitState_DataAvailable && DesiredState == OTGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AOTPlayerState* PS = GetPlayerState<AOTPlayerState>();

		return PS && Manager->HasFeatureReachedInitState(Pawn, UOTPawnExtComponent::NAME_ActorFeatureName, OTGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == OTGameplayTags::InitState_DataInitialized && DesiredState == OTGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UOTHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == OTGameplayTags::InitState_DataAvailable && DesiredState == OTGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AOTPlayerState* PS = GetPlayerState<AOTPlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}

		const UOTPawnData* PawnData = nullptr;

		if (UOTPawnExtComponent* PawnExtComp = UOTPawnExtComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UOTPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(PS->GetOTAbilitySystemComponent(), PS);
		}

		if (AOTPlayerController* PC = GetController<AOTPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
		
	}
}

void UOTHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UOTPawnExtComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == OTGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UOTHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { OTGameplayTags::InitState_Spawned, OTGameplayTags::InitState_DataAvailable, OTGameplayTags::InitState_DataInitialized, OTGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UOTHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UOTPawnExtComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(OTGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UOTHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UOTHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UOTLocalPlayer* LP = Cast<UOTLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UOTPawnExtComponent* PawnExtComp = UOTPawnExtComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UOTPawnData* PawnData = PawnExtComp->GetPawnData<UOTPawnData>())
		{
			if (const UOTInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}
							
							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UOTInputComponent or modify this component accordingly.
				UOTInputComponent* IC = Cast<UOTInputComponent>(PlayerInputComponent);
				if (ensureMsgf(IC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UOTInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					IC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					IC->BindNativeAction(InputConfig, OTGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					IC->BindNativeAction(InputConfig, OTGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

bool UOTHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UOTHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UOTHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UOTHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UOTPawnExtComponent* PawnExtComp = UOTPawnExtComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UOTAbilitySystemComponent* ASC = PawnExtComp->GetOTAbilitySystemComponent())
			{
				ASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UOTHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UOTPawnExtComponent* PawnExtComp = UOTPawnExtComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UOTAbilitySystemComponent* ASC = PawnExtComp->GetOTAbilitySystemComponent())
		{
			ASC->AbilityInputTagReleased(InputTag);
		}
	}
}
