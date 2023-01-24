#pragma once

#include "CoreMinimal.h"
#include "FishBase.h"
#include "FishAi/StimuliObject.h"
#include "FishPike.generated.h"


UCLASS()
class AFishPike : public AFishBase, public IStimuliSource
{
	GENERATED_BODY()

	
protected: // AI

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) override;

private:
	
	int64 lastHuntTime;
	
	bool IsReadyForHunt();

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int minHuntCooldown;
	
public:

	virtual EStimuliObject GetStimuliType() override { return  Pike; }
	
};