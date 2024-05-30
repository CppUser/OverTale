// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "OTCharacter.generated.h"

class UOTPawnExtComponent;

UCLASS()
class OVERTALE_API AOTCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	AOTCharacter(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OT|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOTPawnExtComponent> PawnExtComponent;
};
