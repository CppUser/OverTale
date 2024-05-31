// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/OTInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "System/OTAssetManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"

UOTInputComponent::UOTInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UOTInputComponent::AddInputMappings(const UOTInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required


	//TODO:Temporary bypass Should be handled by user Input Settings
	UOTAssetManager& AssetManager = UOTAssetManager::Get();
	
	if (UEnhancedInputUserSettings* Settings = InputSubsystem->GetUserSettings())
	{
		for (const FInputMappingContextAndPriority& Entry : InputConfig->Mappings)
		{
	
			// Register this IMC with the settings!
			if (UInputMappingContext* IMC = AssetManager.GetAsset(Entry.InputMapping))
			{
				Settings->RegisterInputMappingContext(IMC);
				
				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				// Actually add the config to the local player							
				InputSubsystem->AddMappingContext(IMC, Entry.Priority, Options);
			}

			
		}
	}

							
	
}

void UOTInputComponent::RemoveInputMappings(const UOTInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above


	//TODO:Temporary bypass Should be handled by user Input Settings
	if (UEnhancedInputUserSettings* Settings = InputSubsystem->GetUserSettings())
	{
		for (const FInputMappingContextAndPriority& Entry : InputConfig->Mappings)
		{
			// Skip entries that don't want to be registered
			if (!Entry.bRegisterWithSettings)
			{
				continue;
			}

			// Register this IMC with the settings!
			if (UInputMappingContext* IMC = Entry.InputMapping.Get())
			{
				Settings->UnregisterInputMappingContext(IMC);

				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				// Actually add the config to the local player							
				InputSubsystem->RemoveMappingContext(IMC, Options);
			}
		}
	}
}

void UOTInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}