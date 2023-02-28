#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishSpawner.generated.h"

enum class EFish : uint8;

UCLASS()
class FISHAI_API AFishSpawner : public AActor
{
	GENERATED_BODY()

public:

	class AFishBase* SpawnFish(EFish FishType, FVector Location);

	UPROPERTY(EditDefaultsOnly)
	TMap<EFish, TSubclassOf<AFishBase>> FishBPs;
};
