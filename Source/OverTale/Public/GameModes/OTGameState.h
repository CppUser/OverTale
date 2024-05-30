// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "OTGameState.generated.h"

class UOTExperienceManager;
/**
 * 
 */
UCLASS()
class OVERTALE_API AOTGameState : public AModularGameState
{
	GENERATED_BODY()
public:
	AOTGameState(const FObjectInitializer& ObjInit = FObjectInitializer::Get());
private:
	UPROPERTY()
	TObjectPtr<UOTExperienceManager> ExperienceManager;
	
};
