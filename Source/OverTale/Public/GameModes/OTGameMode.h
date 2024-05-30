// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "OTGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERTALE_API AOTGameMode : public AModularGameMode
{
	GENERATED_BODY()
public:
	AOTGameMode(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	void LoadExperience();
	void OnExperienceLoaded(FPrimaryAssetId ExperienceID,const FString& ExperienceSource);
	
};
