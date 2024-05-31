// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"

#include "OTPlayerController.generated.h"

class UOTAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class OVERTALE_API AOTPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	AOTPlayerController(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "OT|PlayerController")
	AOTPlayerState* GetOTPlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "OT|PlayerController")
	UOTAbilitySystemComponent* GetOTAbilitySystemComponent() const;

protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
