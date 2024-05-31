// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "OTInputUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class OVERTALE_API UOTInputUserSettings : public UEnhancedInputUserSettings
{
	GENERATED_BODY()
public:
	virtual void ApplySettings() override;
};
