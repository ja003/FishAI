// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "FishAi/StimuliObject.h"
#include "FishCarp.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API AFishCarp : public AFishBase, public  IStimuliSource
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg,
		const FHitResult& HitResult);

	virtual void BeginPlay() override;
	
protected: // AI


	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual void OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual EStimuliObject GetStimuliType() override { return EdibleFish; }
	
};
