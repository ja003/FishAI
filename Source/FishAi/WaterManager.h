// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterBodyActor.h"
#include "Fish/FishSpawner.h"


#include "WaterManager.generated.h"

class ANavModifier;
UCLASS()
class FISHAI_API AWaterManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterManager();
	void CalculateCenter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetWaterBounds();

	void ScaleDownWaterBounds();

	FVector2D min;
	FVector2D max;
	FVector center;
	FVector2D center2D;



	void CalculateBoundsInfo();

	void SetPatrolPath();

	void GenerateNavmeshModifiers();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ANavModifier> NavModifierBP;
	
	void GenerateFishes();
	
	void GenerateFishes(EFish FishType);
	
	FVector GetRandomPointInWater(int Counter = 0);
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> GroundObjectType;
	

	UPROPERTY(EditAnywhere)
	bool bDebug_DontGenerateFishes;

	UFUNCTION(BlueprintCallable)
	bool IsPointInWater(FVector point) const;

	FVector GetClosestPointInWater(FVector Point);

	bool UpdateInWaterTarget(FVector& OutTarget);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FVector2D> WaterBounds;
	
	TArray<FVector2D> WaterBoundsOrig;

	TArray<FVector> PatrolPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWaterBody* WaterBody;

	UPROPERTY(EditAnywhere)
	int PatrolPathShoreOffset;

	UPROPERTY(EditAnywhere)
	int ShoreOffset;

	UPROPERTY(EditAnywhere)
	TMap<EFish, int> FishCount;
	
	UPROPERTY(EditAnywhere)
	int PikeCount;

	UPROPERTY(EditAnywhere)
	int CarpCount;

	AFishSpawner* FishSpawner;
	
};
