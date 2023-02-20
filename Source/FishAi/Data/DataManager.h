// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishData.h"
#include "ThrowableData.h"
#include "GameFramework/Actor.h"
#include "DataManager.generated.h"

UCLASS()
class FISHAI_API ADataManager : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<EFish, UFishData*> Fish;

	UPROPERTY(EditDefaultsOnly)
	UThrowableData* Throwable;
};
