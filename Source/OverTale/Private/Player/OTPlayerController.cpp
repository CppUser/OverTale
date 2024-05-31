// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OTPlayerController.h"

#include "AbilitySystem/Components/OTAbilitySystemComponent.h"
#include "Player/OTPlayerState.h"

AOTPlayerController::AOTPlayerController(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
}

AOTPlayerState* AOTPlayerController::GetOTPlayerState() const
{
	return CastChecked<AOTPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UOTAbilitySystemComponent* AOTPlayerController::GetOTAbilitySystemComponent() const
{
	const AOTPlayerState* PS = GetOTPlayerState();
	return (PS ? PS->GetOTAbilitySystemComponent() : nullptr);
}

void AOTPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UOTAbilitySystemComponent* ASC = GetOTAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
