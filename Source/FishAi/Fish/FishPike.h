﻿#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "FishAi/StimuliObject.h"
#include "FishAi/WaterManager.h"
#include "FishPike.generated.h"


UCLASS()
class AFishPike : public AFishBase, public IStimuliSource
{
	GENERATED_BODY()

	
protected: // AI

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

private:

	virtual void BeginPlay() override;
	
	virtual void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult) override;
	
	int64 lastHuntTime;
	
	bool IsReadyForHunt();

	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) override;

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int minHuntCooldown;

	UPROPERTY(EditAnywhere)
	AWaterManager* Water;

	UFUNCTION(BlueprintCallable)
	FVector GetNextPatrolPoint();

private:

	int currentPatrolPathIndex = 0;
	
public:

	virtual EStimuliObject GetStimuliType() override { return Pike; }
	
};