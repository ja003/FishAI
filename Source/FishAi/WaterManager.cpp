// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterManager.h"
#include "Components/SplineComponent.h"

#include "GeomTools.h"


// Sets default values
AWaterManager::AWaterManager()
{
}

// Called when the game starts or when spawned
void AWaterManager::BeginPlay()
{
	Super::BeginPlay();

	SetWaterBounds();

	SetPatrolPath();

	// todo: WaterSpline is incomplete type
	// FVector Location;
	// FVector Tangent;
	// WaterBody->GetWaterSpline()->GetLocationAndTangentAtSplinePoint(0, &Location, &Tangent, ESplineCoordinateSpace::World);
}

void AWaterManager::SetPatrolPath()
{
	FVector center = FVector::ZeroVector;
	for (int i = 0; i < WaterBounds.Num(); i++)
	{
		PatrolPath.Add(FVector(WaterBounds[i].X, WaterBounds[i].Y, 0));
		center += PatrolPath[i];		
	}
	center /= PatrolPath.Num();

	//DrawDebugSphere(GWorld, center, 50, 10, FColor::Blue, true, 5);

	for (int i = PatrolPath.Num() - 1; i >= 0; i--)
	{
		//DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::Red, true, 5);
		
		FVector dirToCenter = center - PatrolPath[i];
		dirToCenter.Normalize();
		PatrolPath[i] = PatrolPath[i] + dirToCenter * PatrolPathShoreOffset;
	}

	// for (int i = 0; i < PatrolPath.Num(); i++)
	// {
	// 	DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::Blue, true, 5);
	// }
}

bool AWaterManager::IsPointInWater(FVector point) const
{
	FString log = point.ToString();
	log += " in ";
	log += "[";
	for(int i = 0; i < WaterBounds.Num(); i++)
	{
		log += "[" + WaterBounds[i].ToString() + "]";
	}
	log += "]";
	log += " = ";
	bool result = FGeomTools2D::IsPointInPolygon(FVector2D(point), WaterBounds);
	log += result ? "TRUE" : "FALSE";

	//UE_LOG(LogTemp, Log, TEXT("xxx %s"), *log);

	return result;
}

