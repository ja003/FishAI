﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishAi/PlayerObjects/EThrowableObject.h"
#include "UObject/Object.h"
#include "ThrowableData.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API UThrowableData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TMap<EThrowableObject, int> NoiseRange;

	UPROPERTY(EditDefaultsOnly)
	int GrenadeRange;
	
};