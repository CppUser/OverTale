// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OTPawnData.generated.h"

class UOTInputConfig;
/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "OverTale Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class OVERTALE_API UOTPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UOTPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from AOTPawn or AOTCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OT|Pawn")
	TSubclassOf<APawn> PawnClass;

	//TODO: // Ability sets to grant to this pawn's ability system.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OT|Abilities")
	// TArray<TObjectPtr<UOTAbilitySet>> AbilitySets;
	//
	//TODO: // What mapping of ability tags to use for actions taking by this pawn
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OT|Abilities")
	// TObjectPtr<UOTAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OT|Input")
	TObjectPtr<UOTInputConfig> InputConfig;

	//TODO: // Default camera mode used by player controlled pawns.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OT|Camera")
	// TSubclassOf<UOTCameraMode> DefaultCameraMode;
};
