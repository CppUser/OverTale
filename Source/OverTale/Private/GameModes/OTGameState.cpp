// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTale/Public/GameModes/OTGameState.h"

#include "GameModes/Components/OTExperienceManager.h"

AOTGameState::AOTGameState(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	ExperienceManager = CreateDefaultSubobject<UOTExperienceManager>(TEXT("ExperienceManager"));
}
