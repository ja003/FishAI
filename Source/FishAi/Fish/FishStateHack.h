// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishStateHack.generated.h"

class AWaterManager;

/**
 * A pike gets stuck sometimes or falls through a navmesh.
 * This component detects the fault state and tries to fix it.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UFishStateHack : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	FVector lastLocation;
	
	int stuckCounter;
	
	void CheckStuck();
	
	bool IsStuck();
	
	void UnStuck();

	void CheckInWater();
	
	void CheckOnNavmesh();

	int outsideWaterCounter;

	UStaticMeshComponent* body;

public:
	
	AWaterManager* Water;

	UPROPERTY(EditAnywhere)
	float MinMoveDistance = 10;
};
