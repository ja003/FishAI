// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterBodyActor.h"
#include "Fish/FishSpawner.h"


#include "WaterManager.generated.h"

UCLASS()
class FISHAI_API AWaterManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetWaterBounds();

	FVector2D min;
	FVector2D max;
	FVector center;
	
	void CalculateBoundsInfo();

	void SetPatrolPath();

	void GenerateFishes();
	
	FVector GetRandomPointInWater(int Counter = 0);
	
public:

	UPROPERTY(EditAnywhere)
	bool bDebug_DontGenerateFishes;

	UFUNCTION(BlueprintCallable)
	bool IsPointInWater(FVector point) const;

	FVector GetInWaterPoint(FVector Point);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FVector2D> WaterBounds;

	TArray<FVector> PatrolPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWaterBody* WaterBody;

	UPROPERTY(EditAnywhere)
	int PatrolPathShoreOffset;

	UPROPERTY(EditAnywhere)
	int PikeCount;

	UPROPERTY(EditAnywhere)
	int CarpCount;

	AFishSpawner* FishSpawner;
	
};
