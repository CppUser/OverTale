// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "OTPawnExtComponent.generated.h"


class UOTPawnData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERTALE_API UOTPawnExtComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UOTPawnExtComponent(const FObjectInitializer& ObjInit = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure)
	static UOTPawnExtComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOTPawnExtComponent>() : nullptr); }
	
	/** The name of this overall feature, this one depends on the other named component features */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

	/** Gets the pawn data, which is used to specify pawn properties in data */
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	/** Sets the current pawn data */
	void SetPawnData(const UOTPawnData* InPawnData);

	/** Should be called by the owning pawn when the pawn's controller changes. */
	void HandleControllerChanged();

	/** Should be called by the owning pawn when the input component is setup. */
	void SetupPlayerInputComponent();
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<const UOTPawnData> PawnData;

};
