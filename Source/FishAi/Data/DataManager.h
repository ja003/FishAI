// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fish/FishData.h"
#include "GameFramework/Actor.h"
#include "Throwable/ThrowableData.h"
#include "DataManager.generated.h"

enum class EThrowableObjectType : uint8;

/*
 * Manager of data assets accessible in scene
 */
UCLASS()
class FISHAI_API ADataManager : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<EFish, UFishData*> Fish;

	UPROPERTY(EditDefaultsOnly)
	TMap<EThrowableObjectType, UThrowableData*> Throwable;
};
