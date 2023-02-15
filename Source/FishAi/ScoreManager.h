// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fish/EFish.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

UCLASS()
class FISHAI_API AScoreManager : public AActor
{
	GENERATED_BODY()

public:

	void OnFishKilled(EFish Fish);

	UPROPERTY(BlueprintReadOnly)
	int Score;
	
};
