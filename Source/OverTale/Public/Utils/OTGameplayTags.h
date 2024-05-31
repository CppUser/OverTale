// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace OTGameplayTags
{
	OVERTALE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	OVERTALE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	OVERTALE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	OVERTALE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	OVERTALE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	OVERTALE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
}
