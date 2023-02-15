#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "FishAi/StimuliObject.h"
#include "Perception/PawnSensingComponent.h"

#include "FishPike.generated.h"


UCLASS()
class AFishPike : public AFishBase, public IStimuliSource
{
	GENERATED_BODY()

	AFishPike();
	
protected: // AI

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

private:
	virtual void BeginPlay() override;
	
	virtual void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult) override;
	
	int64 lastHuntTime;
	
	bool IsReadyForHunt();

	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) override;

	FTimerHandle EndHuntHandle;

	void EndHunt();

	FTimerHandle RoarHandle;
	void Roar();

public:
	/*The function that is going to play the sound and report it to our game*/
	UFUNCTION(BlueprintCallable, Category = AI)
	void ReportNoise(USoundBase* SoundToPlay,float Volume);
 
	/*A Pawn Noise Emitter component which is used in order to emit the sounds to nearby AIs*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

public:
	
	UFUNCTION(BlueprintCallable)
	FVector GetNextPatrolPoint();

private:

	int currentPatrolPathIndex;
	
public:

	virtual EStimuliObject GetStimuliType() override { return Pike; }
	
};