// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoiseReporter.generated.h"

UCLASS()
class FISHAI_API ANoiseReporter : public AActor
{
	GENERATED_BODY()

public:

	/*The function that is going to play the sound and report it to our game*/
	UFUNCTION(BlueprintImplementableEvent)
	void ReportNoise(APawn* InInstigator, FVector InLocation, USoundBase* InSoundToPlay = nullptr , float InLoudness = 1, float InMaxRange = 0, FName InTag = FName());
};
