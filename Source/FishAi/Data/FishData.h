// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FishData.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API UFishData : public UDataAsset
{
	GENERATED_BODY()


public:
	
	UPROPERTY(EditAnywhere)
	int RockRunawayDistance = 500;
};
