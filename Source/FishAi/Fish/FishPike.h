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

public:

	virtual EStimuliObject GetStimuliType() override { return  Pike; }
	
};