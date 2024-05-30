// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OTExperienceSessionSubsystem.generated.h"

UCLASS(BlueprintType)
class UExperienceSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	/** Returns the full map name that will be used during gameplay */
	virtual FString GetMapName() const;

	/** Constructs the full URL that will be passed to ServerTravel */
	virtual FString ConstructTravelURL() const;

	/** Returns true if this request is valid, returns false and logs errors if it is not */
	virtual bool ValidateAndLogErrors(FText& OutError) const;
public:
	/** The map that will be loaded at the start of gameplay, this needs to be a valid Primary Asset top-level map */
	UPROPERTY(BlueprintReadWrite, Category=Session, meta=(AllowedTypes="World"))
	FPrimaryAssetId MapID;

	/** Extra arguments passed as URL options to the game */
	UPROPERTY(BlueprintReadWrite, Category=Session)
	TMap<FString, FString> ExtraArgs;
};

/**
 * 
 */
UCLASS()
class OVERTALE_API UOTExperienceSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UOTExperienceSessionSubsystem() {}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable, Category = Session)
	UExperienceSessionRequest* CreateExperienceSessionRequest();
	
	UFUNCTION(BlueprintCallable, Category=Session)
	virtual void HostSession(APlayerController* HostingPlayer, UExperienceSessionRequest* Request);

};
