// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "FishAi/StimuliObject.h"
#include "FishCarp.generated.h"

/**
 * Randomly swims in the lake. Is attracted to the bait and
 * runs away from a pike and a rock
 */
UCLASS()
class FISHAI_API AFishCarp : public AFishBase, public IStimuliSource
{

	GENERATED_BODY()

private:

	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) override;

protected: // AI

	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual void OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual EStimuliObject GetStimuliType() override { return EdibleFish; }
	
};
