#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "Components/TextRenderComponent.h"
#include "FishAi/StimuliObject.h"

#include "FishPike.generated.h"


class ANoiseReporter;

/*
 * Pike is patrolling randomly between lake spline points.
 * When it sees an edible fish it will hunt it for a while.
 * It gets scared by a rock and a bait.
 **/
UCLASS()
class AFishPike : public AFishBase, public IStimuliSource
{
	GENERATED_BODY()

	AFishPike();
	
protected: // AI

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual void SetState(EFishState NewState) override;

private:
	
	virtual void BeginPlay() override;

	virtual void Init(AWaterManager* InWater) override;

private: // Hunt
	
	int64 lastHuntTime;

	bool IsReadyForHunt();

	FTimerHandle EndHuntHandle;

	void EndHunt();

	FTimerHandle RoarHandle;

	// Makes sound in regular period - scares away fishes
	void Roar();

	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) override;

public:

	// "!" to show that the pike is hunting
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextRenderComponent* StateText;
	
	UFUNCTION(BlueprintCallable)
	FVector GetNextPatrolPoint();

	UPROPERTY(EditAnywhere)
	int currentPatrolPathIndex;
	
public:

	virtual EStimuliObject GetStimuliType() override { return Pike; }
	
};