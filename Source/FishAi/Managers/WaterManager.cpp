// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterManager.h"

#include "FishSpawner.h"
#include "GeomTools.h"
#include "NavModifier.h"
#include "NavModifierVolume.h"
#include "FishAi/Fish/FishBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWaterManager::AWaterManager()
{
}

void AWaterManager::CalculateCenter()
{
	for (int i = 0; i < WaterBounds.Num(); i++)
	{
		FVector point = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);
		center += point;
	}
	center /= WaterBounds.Num();
	center2D = FVector2D(center.X, center.Y);
}

void AWaterManager::OnFishDie(AFishBase* Fish)
{
	int count = Fishes.Num();
	Fishes.Remove(Fish);
	if (ensureMsgf(Fishes.Num() == count - 1, TEXT("fish not removed")))
	{		
		UE_LOG(LogTemp, Log, TEXT("xxx %d fishes left"), Fishes.Num());
	}

	for (AFishBase* fish : Fishes)
	{
		if(fish->Type != EFish::Gold)
			return;
	}

	UE_LOG(LogTemp, Log, TEXT("xxx only gold fishes remain"));
	OnAllFishesDead.Broadcast();
}

// Called when the game starts or when spawned
void AWaterManager::BeginPlay()
{
	Super::BeginPlay();

	FishSpawner = Cast<AFishSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AFishSpawner::StaticClass()));

	SetWaterBounds();

	CalculateCenter();
	
	ScaleDownWaterBounds();
	
	CalculateBoundsInfo();

	SetPatrolPath();

	// todo: WaterSpline is incomplete type
	// FVector Location;
	// FVector Tangent;
	// WaterBody->GetWaterSpline()->GetLocationAndTangentAtSplinePoint(0, &Location, &Tangent, ESplineCoordinateSpace::World);

	GenerateNavmeshModifiers();
	
	if(!bDebug_DontGenerateFishes)
	{
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AWaterManager::GenerateFishes, 1, false);
		//GenerateFishes();
	}
}

void AWaterManager::ScaleDownWaterBounds()
{
	WaterBoundsOrig = WaterBounds;
	TArray<FVector2D> WaterBoundsCopy = WaterBounds;
	//DrawDebugSphere(GWorld, center, 50, 10, FColor::Yellow, false, 5);

	for (int i = 0; i < WaterBoundsCopy.Num(); ++i)
	{
		FVector p = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);
		//DrawDebugSphere(GWorld, p, 50, 10, FColor::Red, true, 5);

		FVector2D prevPoint = WaterBoundsCopy[(WaterBoundsCopy.Num() + i - 1) % WaterBoundsCopy.Num()];
		FVector2D nextPoint = WaterBoundsCopy[(i + 1) % WaterBoundsCopy.Num()];

		//DrawDebugSphere(GWorld, (prevPoint + nextPoint) / 2, 50, 10, FColor::Yellow, false, 50);
		FVector2D dirForward = ((prevPoint + nextPoint) / 2) - WaterBoundsCopy[i];		
		FVector2D dirToCenter = center2D - WaterBoundsCopy[i];
		//DrawDebugLine(GetWorld(), PatrolPath[i], PatrolPath[i] + dirToCenter, FColor::Purple, false, 50);
		//DrawDebugLine(GetWorld(), PatrolPath[i], PatrolPath[i] + dirForward, FColor::Yellow, false, 50);

		float Ang1 = FMath::Atan2(dirForward.X, dirForward.Y);
		float Ang2 = FMath::Atan2(dirToCenter.X, dirToCenter.Y);
		float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
		if(Ang > 180.0f) Ang -= 360.0f; else if(Ang < -180.0f) Ang += 360.0f;

		//UE_LOG(LogTemp, Log, TEXT("xxx Ang = %f"), Ang);
		
		FVector2D moveDir = FMath::Abs(Ang) < 90 ? dirForward : -dirForward;
		
		moveDir.Normalize();
		
		FVector2D movedWaterBound = WaterBoundsCopy[i] + moveDir * ShoreOffset;
		//UpdateInWaterTarget(movedPatrolPoint);

		WaterBounds[i] = movedWaterBound;
	}

	for (int i = 0; i < WaterBounds.Num(); i++)
	{
		FVector p = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);
		//DrawDebugSphere(GWorld, p, 50, 10, FColor::Blue, true, 5);
	}

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

	// for (int i = 0; i < WaterBounds.Num(); i++)
	// {
	// 	FVector patrolPoint = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);
	// 	//center += patrolPoint;
	// 	PatrolPath.Add(patrolPoint);
	// }
	//center /= PatrolPath.Num();
	//center2D = FVector2D(center.X, center.Y);
}

void AWaterManager::SetPatrolPath()
{
	for (int i = 0; i < WaterBounds.Num(); i++)
	{
		FVector patrolPoint = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);
		PatrolPath.Add(patrolPoint);
	}
	
	//DrawDebugSphere(GWorld, center, 50, 10, FColor::Blue, true, 5);
	TArray<FVector> PatrolPathOrig = PatrolPath;


	for (int i = PatrolPath.Num() - 1; i >= 0; i--)
	{
		//DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::White, true, 5);

		FVector prevPoint = PatrolPathOrig[(PatrolPathOrig.Num() + i - 1) % PatrolPathOrig.Num()];
		FVector nextPoint = PatrolPathOrig[(i + 1) % PatrolPathOrig.Num()];

		//DrawDebugSphere(GWorld, (prevPoint + nextPoint) / 2, 50, 10, FColor::Yellow, false, 50);
		FVector dirForward = ((prevPoint + nextPoint) / 2) - PatrolPath[i];		
		FVector dirToCenter = center - PatrolPath[i];
		//DrawDebugLine(GetWorld(), PatrolPath[i], PatrolPath[i] + dirToCenter, FColor::Purple, false, 50);
		//DrawDebugLine(GetWorld(), PatrolPath[i], PatrolPath[i] + dirForward, FColor::Yellow, false, 50);

		float Ang1 = FMath::Atan2(dirForward.X, dirForward.Y);
		float Ang2 = FMath::Atan2(dirToCenter.X, dirToCenter.Y);
		float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
		if(Ang > 180.0f) Ang -= 360.0f; else if(Ang < -180.0f) Ang += 360.0f;

		//UE_LOG(LogTemp, Log, TEXT("xxx Ang = %f"), Ang);
		
		FVector moveDir = FMath::Abs(Ang) < 90 ? dirForward : -dirForward;
		//FVector moveDir = dirForward.Dot(dirToCenter) > 0 ? dirForward : -dirForward;
		
		moveDir.Normalize();
		
		FVector movedPatrolPoint = PatrolPath[i] + moveDir * PatrolPathShoreOffset;
		//DrawDebugSphere(GWorld, movedPatrolPoint, 50, 10, FColor::Black, true, 5);

		UpdateInWaterTarget(movedPatrolPoint);

		PatrolPath[i] = movedPatrolPoint;
	}

	for (int i = 0; i < PatrolPath.Num(); i++)
	{
		//DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::Purple, true, 5);
	}
}

void AWaterManager::GenerateNavmeshModifiers()
{
	for (int i = 0; i < WaterBoundsOrig.Num(); ++i)
	{
		ANavModifier* navMod = GetWorld()->SpawnActor<ANavModifier>(NavModifierBP, FTransform::Identity);
		
		navMod->GenerateBetweenPoints(WaterBoundsOrig[i], WaterBoundsOrig[(i+1)%WaterBoundsOrig.Num()]);
	}
}

void AWaterManager::GenerateFishes()
{
	//UE_LOG(LogTemp, Log, TEXT("xxx GenerateFishes = %d, %d"), PikeCount, CarpCount);

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
			fish->Init(this);
		}

		//UE_LOG(LogTemp, Log, TEXT("xxx Add fish %d"), (int)FishType);
		Fishes.Add(fish);
		//UE_LOG(LogTemp, Log, TEXT("xxx Fishes = %d"), Fishes.Num());
	}
}

FVector AWaterManager::GetRandomPointInWater(int Counter)
{
	FVector point = FVector(FMath::RandRange(min.X, max.X), FMath::RandRange(min.Y, max.Y), 0);
	if (IsPointInWater(point))
	{
		//FVector dirToCenter = center - point;
		//dirToCenter.Normalize();
		//point += dirToCenter * PatrolPathShoreOffset;
		
		//UE_LOG(LogTemp, Log, TEXT("xxx GetRandomPointInWater success on %d"), Counter);
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
			//UE_LOG(LogTemp, Log, TEXT("xxx GetInWaterPoint too many iterations"));
			return center;
		}
	}

	return Point;
}

bool AWaterManager::UpdateInWaterTarget(FVector& OutTarget)
{
	// FHitResult outHit;
	// OutTarget.Z = 0;
	// bool result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), OutTarget, OutTarget + FVector::DownVector * 1000, GroundObjectType, false, TArray<AActor*>(), EDrawDebugTrace::None, outHit, true);
	//
	// if(!result) return false;
	//
	// OutTarget = outHit.Location + FVector::UpVector * 100;

	// more effective
	OutTarget = GetWaterBottomLocation(OutTarget);
	OutTarget += FVector::UpVector * 100;
	
	//DrawDebugSphere(GWorld, OutTarget, 100, 10, FColor::White, false, 5);
	return true;
}



