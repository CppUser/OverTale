// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTale/Public/System/OTAssetManager.h"

const FName FOTBundles::Equipped("Equipped");


UOTAssetManager& UOTAssetManager::Get()
{
	check(GEngine);

	if (UOTAssetManager* Singleton = Cast<UOTAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to OTAssetManager!"));

	return *NewObject<UOTAssetManager>();

}
