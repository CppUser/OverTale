// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"

#include "OTGameInstance.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class OVERTALE_API UOTGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
public:
	UOTGameInstance(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

protected:
	virtual void Init() override;
};
