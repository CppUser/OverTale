// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTale/Public/Character/OTCharacter.h"

#include "AbilitySystem/Components/OTAbilitySystemComponent.h"
#include "OverTale/Public/Character/Components/OTPawnExtComponent.h"

AOTCharacter::AOTCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PawnExtComponent = ObjInit.CreateDefaultSubobject<UOTPawnExtComponent>(this,"PawnExtensionComponent");
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

UOTAbilitySystemComponent* AOTCharacter::GetOTAbilitySystemComponent() const
{
	return Cast<UOTAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AOTCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetOTAbilitySystemComponent();
}

void AOTCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PawnExtComponent->HandleControllerChanged();
}

void AOTCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AOTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AOTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnExtComponent->SetupPlayerInputComponent();
}

