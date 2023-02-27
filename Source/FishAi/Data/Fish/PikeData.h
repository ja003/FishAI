// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishData.h"
#include "PikeData.generated.h"

UCLASS()
class FISHAI_API UPikeData : public UFishData
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditAnywhere)
	int HuntCooldown = 5;

	UPROPERTY(EditAnywhere)
	int HuntDuration = 3;

	UPROPERTY(EditAnywhere)
	int RoarCoolDown = 1;

	UPROPERTY(EditAnywhere)
	int RoarRange = 1500;
	
};
