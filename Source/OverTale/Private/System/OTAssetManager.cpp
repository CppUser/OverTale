// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTale/Public/System/OTAssetManager.h"
#include "Character/OTPawnData.h"

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

const UOTPawnData* UOTAssetManager::GetDefaultPawnData() const
{
	return GetAsset(DefaultPawnData);
}

UObject* UOTAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath,false);
		}
		
		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UOTAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		LoadedAssets.Add(Asset);
	}
}
