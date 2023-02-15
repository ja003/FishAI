﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterManager.h"
#include "GeomTools.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWaterManager::AWaterManager()
{
}

// Called when the game starts or when spawned
void AWaterManager::BeginPlay()
{
	Super::BeginPlay();

	FishSpawner = Cast<AFishSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AFishSpawner::StaticClass()));

	SetWaterBounds();

	CalculateBoundsInfo();

	SetPatrolPath();

	// todo: WaterSpline is incomplete type
	// FVector Location;
	// FVector Tangent;
	// WaterBody->GetWaterSpline()->GetLocationAndTangentAtSplinePoint(0, &Location, &Tangent, ESplineCoordinateSpace::World);

	if(!bDebug_DontGenerateFishes)
		GenerateFishes();
}

void AWaterManager::CalculateBoundsInfo()
{
	for (int i=0; i < WaterBounds.Num(); i++)
	{
		FVector2D point = WaterBounds[i];
		if (point.X < min.X) min.X = point.X;
		if (point.Y < min.Y) min.Y = point.Y;
		if (point.X > max.X) max.X = point.X;
		if (point.Y > max.Y) max.Y = point.Y;	
	}

	for (int i = 0; i < WaterBounds.Num(); i++)
	{
		PatrolPath.Add(FVector(WaterBounds[i].X, WaterBounds[i].Y, 0));
		center += PatrolPath[i];		
	}
	center /= PatrolPath.Num();
}

void AWaterManager::SetPatrolPath()
{

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

void AWaterManager::GenerateFishes()
{
	UE_LOG(LogTemp, Log, TEXT("xxx GenerateFishes = %d, %d"), PikeCount, CarpCount);
	
	for (int i = 0; i < PikeCount; ++i)
	{
		FVector randomPoint = GetRandomPointInWater();
		AFishPike* pike = Cast<AFishPike>(FishSpawner->SpawnFish(EFish::Pike, randomPoint));
		if (ensureMsgf(pike, TEXT("Pike not spawned")))
		{
			pike->Water = this;
		}
	}

	for (int i = 0; i < CarpCount; ++i)
	{
		FVector randomPoint = GetRandomPointInWater();
		AFishCarp* carp = Cast<AFishCarp>(FishSpawner->SpawnFish(EFish::Carp, randomPoint));
		if (ensureMsgf(carp, TEXT("Carp not spawned")))
		{
			carp->Water = this;
		}
	}
}

FVector AWaterManager::GetRandomPointInWater(int Counter)
{
	FVector point = FVector(FMath::RandRange(min.X, max.X), FMath::RandRange(min.Y, max.Y), 0);
	if (IsPointInWater(point))
	{
		UE_LOG(LogTemp, Log, TEXT("xxx GetRandomPointInWater success on %d"), Counter);
		return point;
	}

	if(Counter > 100)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx too many iterations"));
		return center;
	}
	return GetRandomPointInWater(Counter + 1);
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

