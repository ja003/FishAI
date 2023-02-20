// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishData.h"
#include "CarpData.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API UCarpData : public UFishData
{
	GENERATED_BODY()


public:
	
	UPROPERTY(EditAnywhere)
	int PikeRunawayDistance = 800;
	
};
