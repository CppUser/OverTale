// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/Components/OTExperienceManager.h"

#include "GameModes/Components/OTExperienceDefinition.h"
#include "System/OTAssetManager.h"


UOTExperienceManager::UOTExperienceManager(const FObjectInitializer& ObjInit ): Super(ObjInit)
{
}

void UOTExperienceManager::SetCurrentExperience(FPrimaryAssetId ExperienceID)
{
	UOTAssetManager& AssetManager = UOTAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceID);
	TSubclassOf<UOTExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass)

	const UOTExperienceDefinition* Experience = GetDefault<UOTExperienceDefinition>(AssetClass);
	check(Experience);
	check(CurrentExperience == nullptr)
	CurrentExperience = Experience;
	
	
}

void UOTExperienceManager::StartExperienceLoading()
{
	check(CurrentExperience != nullptr);
	check(LoadState == EExperienceLoadState::Unloaded);

	UE_LOG(LogTemp, Log, TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s)"),
		*CurrentExperience->GetPrimaryAssetId().ToString())

	LoadState = EExperienceLoadState::Loading;

	UOTAssetManager& AssetManager = UOTAssetManager::Get();
	
	TSet<FPrimaryAssetId> BundleAssetList;
	TSet<FSoftObjectPath> RawAssetList;

	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	////////////////////////////////////////////
	/// TODO: GameFeatures load bundle
	//Load assets associated with Experience
	TArray<FName> BundlesToLoad;
	BundlesToLoad.Add(FOTBundles::Equipped);
	///////////////////////////////////////////


	TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
	if (BundleAssetList.Num() > 0)
	{
		BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(),BundlesToLoad,{},
			false,FStreamableDelegate(),FStreamableManager::AsyncLoadHighPriority);
	}

	TSharedPtr<FStreamableHandle> RawLoadHandle = nullptr;
	if (RawAssetList.Num() > 0)
	{
		RawLoadHandle = AssetManager.LoadAssetList(RawAssetList.Array(), FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority, TEXT("StartExperienceLoad()"));
	}
	
	// If both async loads are running, combine them
	TSharedPtr<FStreamableHandle> Handle = nullptr;
	if (BundleLoadHandle.IsValid() && RawLoadHandle.IsValid())
	{
		Handle = AssetManager.GetStreamableManager().CreateCombinedHandle({ BundleLoadHandle, RawLoadHandle });
	}
	else
	{
		Handle = BundleLoadHandle.IsValid() ? BundleLoadHandle : RawLoadHandle;
	}

	//Delegate on experice load complete
	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// Assets were already loaded, call the delegate now
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// This set of assets gets preloaded, but we don't block the start of the experience based on it
	TSet<FPrimaryAssetId> PreloadAssetList;
	//@TODO: Determine assets to preload (but not blocking-ly)
	if (PreloadAssetList.Num() > 0)
	{
		AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
	}
}

void UOTExperienceManager::OnExperienceLoadComplete()
{
	check(LoadState == EExperienceLoadState::Loading);
	check(CurrentExperience != nullptr);

	UE_LOG(LogTemp, Log, TEXT("EXPERIENCE: OnExperienceLoadComplete(CurrentExperience = %s)"),
		*CurrentExperience->GetPrimaryAssetId().ToString())
	
	//TODO: Collect GameFeature plugin assets

	//TODO: Do some work on gamefeatures plugin, if we collected and loaded all necesary assets only then call OnExperienceFullLoadComplete 
	OnExperienceFullLoadComplete();
}

void UOTExperienceManager::OnExperienceFullLoadComplete()
{
	check(LoadState != EExperienceLoadState::Loaded);

	//TODO: Do some game feature things with actions

	LoadState = EExperienceLoadState::Loaded;

	//TODO: Apply some shared settings
}
