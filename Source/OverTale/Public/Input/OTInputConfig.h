// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"
#include "OTInputConfig.generated.h"


#pragma region TEMP
//TODO: Move to propper location once user settings implemented
USTRUCT(BlueprintType)
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority = 0;
	
	/** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
	UPROPERTY(EditAnywhere, Category="Input")
	bool bRegisterWithSettings = true;
};
#pragma endregion

USTRUCT(BlueprintType)
struct FOTInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class OVERTALE_API UOTInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UOTInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OT|Character")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	/////////////////////////////////////////////////////////////////
	//TODO: Temp bypass REMOVE once user settings implemented or GameFeatureAction_AddInputContextMapping
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FInputMappingContextAndPriority> Mappings;
	/////////////////////////////////////////////////////////////////
	
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FOTInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FOTInputAction> AbilityInputActions;
};