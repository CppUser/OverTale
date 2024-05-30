// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularGameMode.h"

#include "ModularGameState.h"
#include "ModularPawn.h"
#include "ModularPlayerController.h"

AModularGameMode::AModularGameMode(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	GameStateClass = AModularGameState::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
	
}
