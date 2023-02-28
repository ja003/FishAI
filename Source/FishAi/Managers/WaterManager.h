// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterBodyActor.h"
#include "FishSpawner.h"
#include "FishAi/Throwable/ThrowablesGenerator.h"


#include "WaterManager.generated.h"

class UWaterData;
enum class EFish : uint8;
class AFishBase;
class ANavModifier;

/**
 * Manages one water area (lake) and all fishes in it 
 */
UCLASS()
class FISHAI_API AWaterManager : public AActor
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	void Init();
	
public: // fishes
	
	void OnFishDie(AFishBase* Fish);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllFishesDead);
	UPROPERTY(BlueprintAssignable)
	FOnAllFishesDead OnAllFishesDead;

	void GenerateFish(EFish FishType);
	void GenerateFish(EFish FishType, FVector Location);
	
private:
	
	TArray<AFishBase*> Fishes;
		
	void GenerateFishes();
	void GenerateFishes(EFish FishType);

protected: // bounds

	// Reads info from spline defining the water
	UFUNCTION(BlueprintImplementableEvent)
	void SetWaterBounds();

	void CalculateCenter();

	// Defining spline is much larger than the actual water.
	// Bounds have to be scaled down so they represent watter a bit better
	void ScaleDownWaterBounds();

	FVector2D min;
	FVector2D max;
	FVector center;
	FVector2D center2D;	

	void CalculateBoundsMinMax();

	// Sets patrol points used by a Pike.
	// Patrol points are bounds point moved a bit to the center.
	void SetPatrolPath();

	// Generates obstacles preventing fishes to leave a water 
	void GenerateNavmeshModifiers();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ANavModifier> NavModifierBP;

	// Finds generators in scene having the same tag as this object
	// and assigns them
	void SetGenerators();
	
public: // water 

	// this doesnt include waves:(
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetWaterSurfaceLocation(FVector AtLocation);
	
	FVector GetRandomPointInWater(int Counter = 0);
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> GroundObjectType;
	

	UFUNCTION(BlueprintCallable)
	bool IsPointInWater(FVector point) const;

	FVector GetClosestPointInWater(FVector Point);

	// Updates target to be at the bottom of a water so navmesh
	// agents can properly reach it
	bool UpdateInWaterTarget(FVector& OutTarget);

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetWaterBottomLocation(FVector AtLocation);

public:

	UPROPERTY(EditAnywhere)
	bool bDebug_SelfInit;

	// Scaled down water bounds (estimated to better match the actual water)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FVector2D> WaterBounds;

	// Original water bounds
	TArray<FVector2D> WaterBoundsOrig;

	// Patrol path used by a pike
	TArray<FVector> PatrolPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWaterBody* WaterBody;

	// Offset from a shore to the patrol path
	UPROPERTY(EditAnywhere)
	int PatrolPathShoreOffset;

	// Estimated offset from defining water spline to the shore 
	UPROPERTY(EditAnywhere)
	int ShoreOffset;

	UPROPERTY(EditAnywhere)
	UWaterData* Data;
	
	AFishSpawner* FishSpawner;

private:

	// Throwable generators at this water
	TArray<AThrowablesGenerator*> Generators;
	
};
