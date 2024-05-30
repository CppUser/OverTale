// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OTExperienceDefinition.generated.h"

class UOTPawnData;
/**
 * 
 */
UCLASS(BlueprintType,Const)
class OVERTALE_API UOTExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	//TODO: GameFeatures to enable
	//TODO: Feature Actions
	//TODO: Feature Action Sets
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<UOTPawnData> PawnClass;
};
