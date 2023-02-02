// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowPrediction.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Called every frame
void AThrowPrediction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowPrediction::CreateSpline()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if(SplineComponent)
	{
		SetRootComponent(SplineComponent);  
	}

	EndPoint = GetWorld()->SpawnActor(EndPointBP->GetClass());
	EndPoint->SetHidden(true);
	
}

void AThrowPrediction::DestroySpline()
{
	if(SplineComponent == nullptr) return;

	SplineComponent->DestroyComponent();
	SplineComponent = nullptr;
	EndPoint->Destroy();
	EndPoint = nullptr;
	
}

void AThrowPrediction::DestroyMeshes()
{
	for (auto splineMesh : SplineMeshes)
	{
		splineMesh->DestroyComponent();
	}
	SplineMeshes.Empty();
}

void AThrowPrediction::DrawSpline()
{
	if(SplineComponent == nullptr) return;

	FPredictProjectilePathParams params;
	params.StartLocation = Start;
	params.LaunchVelocity = Velocity;
	params.ActorsToIgnore = IgnoredActors;
	params.ProjectileRadius = 25;
	FPredictProjectilePathResult predictResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), params, predictResult);

	// ...
}
