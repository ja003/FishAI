// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFish.h"
#include "FishCarp.h"
#include "FishPike.h"
#include "GameFramework/Actor.h"
#include "FishSpawner.generated.h"

UCLASS()
class FISHAI_API AFishSpawner : public AActor
{
	GENERATED_BODY()

public:

	AFishBase* SpawnFish(EFish FishType, FVector Location);
	

	UPROPERTY(EditDefaultsOnly)
	TMap<EFish, TSubclassOf<AFishBase>> FishBPs;
};
