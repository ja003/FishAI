// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ThrowableData.generated.h"

/**
 * Base data class for throwable objects
 */
UCLASS()
class FISHAI_API UThrowableData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	int NoiseRange;

	// Offset of spawned object to visually fit in hand of a player
	UPROPERTY(EditAnywhere)
	FTransform SpawnOffset;

	// Rotation applied during Tick after the object if thrown 
	UPROPERTY(EditAnywhere)
	FRotator TickRotation;
};
