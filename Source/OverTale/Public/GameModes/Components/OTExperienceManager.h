// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "OTExperienceManager.generated.h"


class UOTExperienceDefinition;

enum class EExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERTALE_API UOTExperienceManager : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UOTExperienceManager(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

	void SetCurrentExperience(FPrimaryAssetId ExperienceID);

private:
	void StartExperienceLoading();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadComplete();

private:
	UPROPERTY()
	TObjectPtr<const UOTExperienceDefinition> CurrentExperience;
	
	EExperienceLoadState LoadState = EExperienceLoadState::Unloaded;
};
