#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "Components/TextRenderComponent.h"
#include "FishAi/StimuliObject.h"

#include "FishPike.generated.h"


class ANoiseReporter;
UCLASS()
class AFishPike : public AFishBase, public IStimuliSource
{
	GENERATED_BODY()

	AFishPike();
	
protected: // AI

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

	virtual void SetState(EFishState NewState) override;

private:
	virtual void BeginPlay() override;
	
	virtual void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult) override;
	
	int64 lastHuntTime;

	UPROPERTY(EditDefaultsOnly)
	UTextRenderComponent* StateText;

	bool IsReadyForHunt();

	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) override;

	FTimerHandle EndHuntHandle;

	void EndHunt();

	FTimerHandle RoarHandle;
	void Roar();

public:
	
	UFUNCTION(BlueprintCallable)
	FVector GetNextPatrolPoint();

//private:

	UPROPERTY(EditAnywhere)
	int currentPatrolPathIndex;
	
public:

	virtual EStimuliObject GetStimuliType() override { return Pike; }
	
};