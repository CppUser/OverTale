// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "OTPlayerState.generated.h"

class UOTAbilitySystemComponent;
class UOTExperienceDefinition;
class UOTPawnData;
/**
 * 
 */
UCLASS()
class OVERTALE_API AOTPlayerState : public AModularPlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AOTPlayerState(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	UOTAbilitySystemComponent* GetOTAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/** Gets the pawn data, which is used to specify pawn properties in data */
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	/** Sets the current pawn data */
	void SetPawnData(const UOTPawnData* InPawnData);

	
protected:
	virtual void PostInitializeComponents() override;

private:
	void OnExperienceLoaded(const UOTExperienceDefinition* CurrentExperience);

public:
	static const FName NAME_OTAbilityReady;
protected:
	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY()
	TObjectPtr<const UOTPawnData> PawnData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UOTAbilitySystemComponent> ASC;
};
