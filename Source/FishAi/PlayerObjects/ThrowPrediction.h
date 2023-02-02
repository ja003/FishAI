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
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void CreateSpline();

	void DestroySpline();

	void DestroyMeshes();

	void DrawSpline();

	void SetEnabled(bool bEnabled);

	void UpdateValues();

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;

public:
	
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EndPointBP;

	AActor* EndPoint;

	FVector Start;
	FVector Velocity;

	bool bIsEnabled;

	TArray<AActor*> IgnoredActors;
	
};