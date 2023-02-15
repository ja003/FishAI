// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RewardText.h"
#include "Fish/EFish.h"
#include "Fish/FishBase.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

UCLASS()
class FISHAI_API AScoreManager : public AActor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	void OnFishKilled(AFishBase* Fish);

	UPROPERTY(BlueprintReadOnly)
	int Score;

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	TSubclassOf<ARewardText> RewardTextBP;

	ADataManager* Data;

	
};
