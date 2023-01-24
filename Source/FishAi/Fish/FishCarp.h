// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "FishCarp.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API AFishCarp : public AFishBase
{
	GENERATED_BODY()

	
protected: // AI


	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual void OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;
	
};
