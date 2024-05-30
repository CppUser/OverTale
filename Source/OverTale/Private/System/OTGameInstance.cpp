// Fill out your copyright notice in the Description page of Project Settings.


#include "System/OTGameInstance.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Utils/OTGameplayTags.h"

UOTGameInstance::UOTGameInstance(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
}

void UOTGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* GFC = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(GFC))
	{
		GFC->RegisterInitState(OTGameplayTags::InitState_Spawned,false,FGameplayTag());
		GFC->RegisterInitState(OTGameplayTags::InitState_DataAvailable,false,OTGameplayTags::InitState_Spawned);
		GFC->RegisterInitState(OTGameplayTags::InitState_DataInitialized,false,OTGameplayTags::InitState_DataAvailable);
		GFC->RegisterInitState(OTGameplayTags::InitState_GameplayReady,false,OTGameplayTags::InitState_DataInitialized);
	}
}
