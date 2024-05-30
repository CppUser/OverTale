// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "OTWorldSettings.generated.h"

class UOTExperienceDefinition;
/**
 * 
 */
UCLASS()
class OVERTALE_API AOTWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	AOTWorldSettings(const FObjectInitializer& ObjInit);

	FPrimaryAssetId GetDefaultGameplayExperience() const;
protected:
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<UOTExperienceDefinition> DefaultGameplayExperience;
};
