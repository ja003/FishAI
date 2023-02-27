// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishAi/Fish/EFish.h"
#include "UObject/Object.h"
#include "FishData.generated.h"

/**
 * Base data class for fishes
 */
UCLASS()
class FISHAI_API UFishData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int Speed = 200;
	
	UPROPERTY(EditAnywhere)
	int RockRunawayDistance = 800;
	
	UPROPERTY(EditAnywhere)
	int Reward;

	UPROPERTY(EditAnywhere)
	int SightRadius = 1000;

	UPROPERTY(EditAnywhere)
	int HearRadius = 1000;
	
};
