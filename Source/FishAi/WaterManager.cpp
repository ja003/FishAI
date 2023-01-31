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

	// todo: WaterSpline is incomplete type
	// FVector Location;
	// FVector Tangent;
	// WaterBody->GetWaterSpline()->GetLocationAndTangentAtSplinePoint(0, &Location, &Tangent, ESplineCoordinateSpace::World);
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

	UE_LOG(LogTemp, Log, TEXT("xxx %s"), *log);

	return result;
}

