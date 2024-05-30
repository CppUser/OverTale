// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Components/OTExperienceHandler.h"
#include "Engine/Engine.h"
#include "Subsystems/SubsystemCollection.h"

#if WITH_EDITOR

void UOTExperienceHandler::OnPlayInEditorBegun()
{
	ensure(GameFeaturePluginRequestCountMap.IsEmpty());
	GameFeaturePluginRequestCountMap.Empty();
}

void UOTExperienceHandler::NotifyOfPluginActivation(const FString PluginURL)
{
	if (GIsEditor)
	{
		UOTExperienceHandler* ExperienceManagerSubsystem = GEngine->GetEngineSubsystem<UOTExperienceHandler>();
		check(ExperienceManagerSubsystem);

		// Track the number of requesters who activate this plugin. Multiple load/activation requests are always allowed because concurrent requests are handled.
		int32& Count = ExperienceManagerSubsystem->GameFeaturePluginRequestCountMap.FindOrAdd(PluginURL);
		++Count;
	}
}

bool UOTExperienceHandler::RequestToDeactivatePlugin(const FString PluginURL)
{
	if (GIsEditor)
	{
		UOTExperienceHandler* ExperienceManagerSubsystem = GEngine->GetEngineSubsystem<UOTExperienceHandler>();
		check(ExperienceManagerSubsystem);

		// Only let the last requester to get this far deactivate the plugin
		int32& Count = ExperienceManagerSubsystem->GameFeaturePluginRequestCountMap.FindChecked(PluginURL);
		--Count;

		if (Count == 0)
		{
			ExperienceManagerSubsystem->GameFeaturePluginRequestCountMap.Remove(PluginURL);
			return true;
		}

		return false;
	}

	return true;
}

#endif