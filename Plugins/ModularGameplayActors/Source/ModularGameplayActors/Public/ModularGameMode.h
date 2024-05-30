// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ModularGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MODULARGAMEPLAYACTORS_API AModularGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AModularGameMode(const FObjectInitializer& ObjInit = FObjectInitializer::Get());
};
