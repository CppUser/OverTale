// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "OTAssetManager.generated.h"


struct FOTBundles
{
	static const FName Equipped;
};

/**
 * 
 */
UCLASS(Config = Game)
class OVERTALE_API UOTAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	static UOTAssetManager& Get();
};
