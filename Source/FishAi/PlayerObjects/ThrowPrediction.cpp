// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrowPrediction.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AThrowPrediction::AThrowPrediction()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if(SplineComponent)
	{
		SetRootComponent(SplineComponent);  
	}
}

// Called every frame
void AThrowPrediction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsEnabled)
	{
		DrawSpline();
	}
}

void AThrowPrediction::BeginPlay()
{
	Super::BeginPlay();

	EndPoint = GetWorld()->SpawnActor(EndPointBP);
	EndPoint->SetActorHiddenInGame(true);
}

void AThrowPrediction::DestroyMeshes()
{
	for (auto splineMesh : SplineMeshes)
	{
		splineMesh->DestroyComponent();
	}
	SplineMeshes.Empty();
}

bool AThrowPrediction::CalculateSplinePath()
{
	FPredictProjectilePathParams params;
	params.StartLocation = Start;
	params.LaunchVelocity = Velocity;
	params.ActorsToIgnore = IgnoredActors;
	params.ProjectileRadius = 25;
	//params.TraceChannel = ECC_WorldStatic;
	
	//params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	params.bTraceComplex = false;
	//params.bTraceWithChannel = true;
	params.bTraceWithCollision = true; //important
	
	params.SimFrequency = 15;
	params.MaxSimTime = 5;
	
	FPredictProjectilePathResult predictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(GetWorld(), params, predictResult);

	// DrawDebugSphere(GWorld, Start, 5, 10, FColor::Blue, false, .1f);
	// DrawDebugLine(GWorld, Start, Start + Velocity, FColor::Red);

	EndPoint->SetActorHiddenInGame(!bHit);
	DestroyMeshes();

	if(!bHit)
		return false;
	
	EndPoint->SetActorLocation(predictResult.HitResult.ImpactPoint);
	TArray<FVector> splinePoints;
	for(int i = 0; i < predictResult.PathData.Num(); i++)
	{
		splinePoints.Add(predictResult.PathData[i].Location);
	}
	SplineComponent->SetSplinePoints(splinePoints, ESplineCoordinateSpace::World);
	return true;
}

void AThrowPrediction::DrawSpline()
{
	if(SplineComponent == nullptr) return;

	if (!CalculateSplinePath()) return;

	constexpr int spline_part_lenght = 100;
	for (int i = 0; i < SplineComponent->GetSplineLength() / spline_part_lenght; i++)
	{		
		USplineMeshComponent *SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->RegisterComponent();
		SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;


		FVector startLocation = SplineComponent->GetLocationAtDistanceAlongSpline(i * spline_part_lenght, ESplineCoordinateSpace::World);
		FVector startTangent = SplineComponent->GetTangentAtDistanceAlongSpline(i * spline_part_lenght, ESplineCoordinateSpace::World);
		startTangent = startTangent.GetClampedToSize(0, spline_part_lenght);

		FVector endLocation = SplineComponent->GetLocationAtDistanceAlongSpline((i + 1) * spline_part_lenght, ESplineCoordinateSpace::World);
		FVector endTangent = SplineComponent->GetTangentAtDistanceAlongSpline((i + 1) * spline_part_lenght, ESplineCoordinateSpace::World);
		endTangent = startTangent.GetClampedToSize(0, spline_part_lenght);
		
		SplineMesh->SetStartAndEnd(startLocation, startTangent, endLocation, endTangent);

		constexpr float mesh_scale = .1f;
		SplineMesh->SetStartScale(FVector2D(mesh_scale, mesh_scale));
		SplineMesh->SetEndScale(FVector2D(mesh_scale, mesh_scale));

		SplineMesh->SetMobility(EComponentMobility::Movable);
		
		SplineMesh->SetStaticMesh(SplinePartMesh);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
		SplineMesh->SetMaterial(0, SplinePartMaterial);

		SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		SplineMesh->SetCastShadow(false);

		SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		
		SplineMeshes.Add(SplineMesh);
	}	
}

void AThrowPrediction::SetEnabled(bool bEnabled)
{
	bIsEnabled = bEnabled;

	if(bEnabled)
	{
		EndPoint->SetActorHiddenInGame(true);
	}
	else
	{
		EndPoint->SetActorHiddenInGame(true);
		DestroyMeshes();
	}
}

void AThrowPrediction::UpdateValues(FVector InStart, FVector InVelocity, TArray<AActor*> InIgnoredActors)
{
	Start = InStart;
	Velocity = InVelocity;
	IgnoredActors = InIgnoredActors;
}
