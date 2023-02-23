// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishAi/NextLevelTrigger.h"
#include "UObject/Object.h"
#include "GameManager.generated.h"

class AWaterManager;
/**
 * 
 */
UCLASS()
class FISHAI_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnAllFishesDead();

	int currentLevel = -1;

	UFUNCTION()
	void OnNextLevelTriggerOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);

	void TriggerNextLevel();


	UPROPERTY(EditAnywhere)
	TArray<AWaterManager*> WaterManagers;

	UPROPERTY(EditAnywhere)
	TArray<ANextLevelTrigger*> NextLevelTriggers;
	
};
