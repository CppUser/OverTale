// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/OTInputConfig.h"

UOTInputConfig::UOTInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UOTInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FOTInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		//TODO: log in  OTLog channel
		UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}