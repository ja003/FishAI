// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishAi/Fish/EFish.h"
#include "UObject/Object.h"
#include "WaterData.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API UWaterData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TMap<EFish, int> FishCount;

	UPROPERTY(EditAnywhere)
	bool bDebug_DontGenerateFishes;
};
