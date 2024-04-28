// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverTaleGameMode.h"
#include "OverTaleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverTaleGameMode::AOverTaleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
