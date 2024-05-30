// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OTPawnData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "OverTale Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class OVERTALE_API UOTPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OT|Pawn")
	TSubclassOf<APawn> PawnClass;
};
