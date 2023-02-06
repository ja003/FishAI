// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowPrediction.generated.h"

class USplineMeshComponent;
class USplineComponent;
UCLASS()
class FISHAI_API AThrowPrediction : public AActor
{
private:
	GENERATED_BODY()

public:
	
	AThrowPrediction();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void SetEnabled(bool bEnabled);

	void UpdateValues(FVector InStart, FVector InVelocity, TArray<AActor*> InIgnoredActors);
	
private:

	void DestroyMeshes();
	bool CalculateSplinePath();

	void DrawSpline();

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;

public:

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* SplinePartMaterial;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* SplinePartMesh;
	
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EndPointBP;

	AActor* EndPoint;

	FVector Start;
	FVector Velocity;

	bool bIsEnabled;

	TArray<AActor*> IgnoredActors;
	
};