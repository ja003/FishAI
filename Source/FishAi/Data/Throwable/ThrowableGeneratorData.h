#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ThrowableGeneratorData.generated.h"

UCLASS()
class UThrowableGeneratorData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int Cooldown;
	
};
