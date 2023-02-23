// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishStateHack.generated.h"


class AWaterManager;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UFishStateHack : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFishStateHack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector lastLocation;
	
	int stuckCounter;
	
	void CheckStuck();
	
	bool IsStuck();
	
	void UnStuck();

	void CheckInWater();

	int outsideWaterCounter;

	UStaticMeshComponent* body;

public:
	AWaterManager* Water;

	UPROPERTY(EditAnywhere)
	float MinMoveDistance = 10;
};
