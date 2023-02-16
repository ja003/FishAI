// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterManager.h"
#include "GeomTools.h"
#include "NavModifier.h"
#include "NavModifierVolume.h"
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

	GenerateNavmeshModifiers();
	
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
		FVector patrolPoint = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);
		center += patrolPoint;
		PatrolPath.Add(patrolPoint);
	}
	center /= PatrolPath.Num();
}

void AWaterManager::SetPatrolPath()
{

	//DrawDebugSphere(GWorld, center, 50, 10, FColor::Blue, true, 5);
	TArray<FVector> PatrolPathOrig = PatrolPath;


	for (int i = PatrolPath.Num() - 1; i >= 0; i--)
	{
		//DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::Red, true, 5);

		FVector prevPoint = PatrolPathOrig[(PatrolPathOrig.Num() + i - 1) % PatrolPathOrig.Num()];
		FVector nextPoint = PatrolPathOrig[(i + 1) % PatrolPathOrig.Num()];

		//DrawDebugSphere(GWorld, (prevPoint + nextPoint) / 2, 50, 10, FColor::Yellow, false, 50);
		FVector dirForward = ((prevPoint + nextPoint) / 2) - PatrolPath[i];		
		FVector dirToCenter = center - PatrolPath[i];
		//DrawDebugLine(GetWorld(), PatrolPath[i], PatrolPath[i] + dirToCenter, FColor::Purple, false, 50);
		//DrawDebugLine(GetWorld(), PatrolPath[i], PatrolPath[i] + dirForward, FColor::Yellow, false, 50);
		
		FVector moveDir = dirForward.Dot(dirToCenter) > 0 ? dirForward : dirToCenter;
		
		moveDir.Normalize();
		
		FVector movedPatrolPoint = PatrolPath[i] + moveDir * PatrolPathShoreOffset;
		UpdateInWaterTarget(movedPatrolPoint);

		PatrolPath[i] = movedPatrolPoint;
	}

	for (int i = 0; i < PatrolPath.Num(); i++)
	{
		//DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::Blue, true, 5);
	}
}

void AWaterManager::GenerateNavmeshModifiers()
{
	for (int i = 0; i < WaterBounds.Num(); ++i)
	{
		ANavModifier* navMod = GetWorld()->SpawnActor<ANavModifier>(NavModifierBP, FTransform::Identity);
		
		navMod->GenerateBetweenPoints(WaterBounds[i], WaterBounds[(i+1)%WaterBounds.Num()]);
	}
}

void AWaterManager::GenerateFishes()
{
	UE_LOG(LogTemp, Log, TEXT("xxx GenerateFishes = %d, %d"), PikeCount, CarpCount);

	GenerateFishes(EFish::Carp);
	GenerateFishes(EFish::Pike);
	GenerateFishes(EFish::Gold);
}

void AWaterManager::GenerateFishes(EFish FishType)
{
	if (!FishCount.Contains(FishType))
		return;

	for (int i = 0; i < FishCount[FishType]; ++i)
	{
		FVector randomPoint = GetRandomPointInWater();
		AFishBase* fish = Cast<AFishBase>(FishSpawner->SpawnFish(FishType, randomPoint));
		if (ensureMsgf(fish, TEXT("fish not spawned")))
		{
			fish->Water = this;
		}
	}
}

FVector AWaterManager::GetRandomPointInWater(int Counter)
{
	FVector point = FVector(FMath::RandRange(min.X, max.X), FMath::RandRange(min.Y, max.Y), 0);
	if (IsPointInWater(point))
	{
		FVector dirToCenter = center - point;
		dirToCenter.Normalize();
		point += dirToCenter * PatrolPathShoreOffset;
		
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

FVector AWaterManager::GetClosestPointInWater(FVector Point)
{
	Point.Z = 0;
	int counter = 0;
	while(!IsPointInWater(Point))
	{
		FVector dirToCenter = center - Point;
		dirToCenter.Normalize();
		Point += dirToCenter * 100;
		counter++;
		if (counter > 20)
		{
			UE_LOG(LogTemp, Log, TEXT("xxx GetInWaterPoint too many iterations"));
			return center;
		}
	}

	return Point;
}

bool AWaterManager::UpdateInWaterTarget(FVector& OutTarget)
{
	FHitResult outHit;
	bool result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), OutTarget, OutTarget + FVector::DownVector * 1000, GroundObjectType, false, TArray<AActor*>(), EDrawDebugTrace::None, outHit, true);

	if(!result) return false;

	OutTarget = outHit.Location;
	DrawDebugSphere(GWorld, OutTarget, 100, 10, FColor::White, false, .5f);
	return true;
}



