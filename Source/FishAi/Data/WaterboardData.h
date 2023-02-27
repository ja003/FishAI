﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WaterboardData.generated.h"

UCLASS()
class FISHAI_API UWaterboardData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	float ForwardSpeed;

	UPROPERTY(EditDefaultsOnly)
	float TurnSpeed;
	
};
