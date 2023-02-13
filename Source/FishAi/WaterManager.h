// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterBodyActor.h"


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

	void SetPatrolPath();
	
public:

	UFUNCTION(BlueprintCallable)
	bool IsPointInWater(FVector point) const;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FVector2D> WaterBounds;

	TArray<FVector> PatrolPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWaterBody* WaterBody;

	UPROPERTY(EditAnywhere)
	int PatrolPathShoreOffset;
};
