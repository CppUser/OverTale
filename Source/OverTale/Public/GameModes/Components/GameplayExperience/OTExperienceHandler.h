﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Subsystems/EngineSubsystem.h"
#include "OTExperienceHandler.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UOTExperienceHandler : public UEngineSubsystem
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	OVERTALE_API void OnPlayInEditorBegun();

	static void NotifyOfPluginActivation(const FString PluginURL);
	static bool RequestToDeactivatePlugin(const FString PluginURL);
#else
	static void NotifyOfPluginActivation(const FString PluginURL) {}
	static bool RequestToDeactivatePlugin(const FString PluginURL) { return true; }
#endif

private:
	// The map of requests to active count for a given game feature plugin
	// (to allow first in, last out activation management during PIE)
	TMap<FString, int32> GameFeaturePluginRequestCountMap;
};
