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
	
public:

	virtual EStimuliObject GetStimuliType() override { return  Pike; }
	
};