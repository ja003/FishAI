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

UCLASS()
class FISHAI_API AWaterManager : public AActor
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	AWaterManager();
	void CalculateCenter();
	
	void OnFishDie(AFishBase* Fish);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllFishesDead);
	UPROPERTY(BlueprintAssignable)
	FOnAllFishesDead OnAllFishesDead;

	void Init();
	
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
	
	TArray<AFishBase*> Fishes;

	void CalculateBoundsInfo();

	void SetPatrolPath();

	void GenerateNavmeshModifiers();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ANavModifier> NavModifierBP;
	
	void GenerateFishes();
	
	void GenerateFishes(EFish FishType);
	
	
public:

	// this doesnt include waves:(
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetWaterSurfaceLocation(FVector AtLocation);
	
	FVector GetRandomPointInWater(int Counter = 0);
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> GroundObjectType;
	

	UFUNCTION(BlueprintCallable)
	bool IsPointInWater(FVector point) const;

	FVector GetClosestPointInWater(FVector Point);

	bool UpdateInWaterTarget(FVector& OutTarget);

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetWaterBottomLocation(FVector AtLocation);

public:

	UPROPERTY(EditAnywhere)
	bool bDebug_SelfInit;

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
	UWaterData* Data;
	
	AFishSpawner* FishSpawner;

	UPROPERTY(EditAnywhere)
	TArray<AThrowablesGenerator*> Generators;
	
};
