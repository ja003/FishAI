// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterManager.h"

#include "FishSpawner.h"
#include "GeomTools.h"
#include "NavModifier.h"
#include "NavModifierVolume.h"
#include "FishAi/Data/WaterData.h"
#include "FishAi/Fish/FishBase.h"
#include "Kismet/GameplayStatics.h"


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
		if(fish->FishType != EFish::Gold)
			return;
	}

	OnAllFishesDead.Broadcast();
	
	for (auto generator : Generators)
	{
		generator->Disable();
	}
}

void AWaterManager::Init()
{
	FishSpawner = Cast<AFishSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AFishSpawner::StaticClass()));

	SetWaterBounds();

	CalculateCenter();
	
	ScaleDownWaterBounds();
	
	CalculateBoundsMinMax();

	SetPatrolPath();

	SetGenerators();

	GenerateNavmeshModifiers();
	
	if(!Data->bDebug_DontGenerateFishes)
	{
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AWaterManager::GenerateFishes, 1, false);
	}
}

void AWaterManager::BeginPlay()
{
	Super::BeginPlay();

	if (bDebug_SelfInit)
	{
		Init();
	}
}

void AWaterManager::ScaleDownWaterBounds()
{
	WaterBoundsOrig = WaterBounds;
	TArray<FVector2D> WaterBoundsCopy = WaterBounds;

	for (int i = 0; i < WaterBoundsCopy.Num(); ++i)
	{
		FVector p = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);

		FVector2D prevPoint = WaterBoundsCopy[(WaterBoundsCopy.Num() + i - 1) % WaterBoundsCopy.Num()];
		FVector2D nextPoint = WaterBoundsCopy[(i + 1) % WaterBoundsCopy.Num()];

		FVector2D dirForward = ((prevPoint + nextPoint) / 2) - WaterBoundsCopy[i];		
		FVector2D dirToCenter = center2D - WaterBoundsCopy[i];

		float Ang1 = FMath::Atan2(dirForward.X, dirForward.Y);
		float Ang2 = FMath::Atan2(dirToCenter.X, dirToCenter.Y);
		float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
		if(Ang > 180.0f) Ang -= 360.0f; else if(Ang < -180.0f) Ang += 360.0f;

		FVector2D moveDir = FMath::Abs(Ang) < 90 ? dirForward : -dirForward;
		
		moveDir.Normalize();
		
		FVector2D movedWaterBound = WaterBoundsCopy[i] + moveDir * ShoreOffset;

		WaterBounds[i] = movedWaterBound;
	}

	for (int i = 0; i < WaterBounds.Num(); i++)
	{
		FVector p = FVector(WaterBounds[i].X, WaterBounds[i].Y, 0);

		if(i < WaterBounds.Num() - 1)
		{
			FVector p2 = FVector(WaterBounds[i+1].X, WaterBounds[i+1].Y, 0);
		}
	}
}

void AWaterManager::CalculateBoundsMinMax()
{
	for (int i=0; i < WaterBounds.Num(); i++)
	{
		FVector2D point = WaterBounds[i];
		if (point.X < min.X) min.X = point.X;
		if (point.Y < min.Y) min.Y = point.Y;
		if (point.X > max.X) max.X = point.X;
		if (point.Y > max.Y) max.Y = point.Y;	
	}
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
		//DrawDebugSphere(GWorld, PatrolPath[i], 50, 10, FColor::Red, true, 5);
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

void AWaterManager::SetGenerators()
{
	if (Tags.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("xxx error: WaterManager doesnt have any tag"));
		return;
	}
	TArray<AActor*> generators;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tags[0], generators);

	for (auto generator : generators)
	{
		if (AThrowablesGenerator* throwGen = Cast<AThrowablesGenerator>(generator))
			Generators.Add(throwGen);
	}
}

void AWaterManager::GenerateFishes()
{
	GenerateFishes(EFish::Carp);
	GenerateFishes(EFish::Pike);
	GenerateFishes(EFish::Gold);
}

void AWaterManager::GenerateFish(EFish FishType, FVector Location)
{
	AFishBase* fish = Cast<AFishBase>(FishSpawner->SpawnFish(FishType, Location));
	if (ensureMsgf(fish, TEXT("fish not spawned")))
	{
		fish->Init(this);
		Fishes.Add(fish);
	}
}

void AWaterManager::GenerateFish(EFish FishType)
{
	FVector randomPoint = GetRandomPointInWater();
	GenerateFish(FishType, randomPoint);
}

void AWaterManager::GenerateFishes(EFish FishType)
{
	if (!Data->FishCount.Contains(FishType))
		return;

	for (int i = 0; i < Data->FishCount[FishType]; ++i)
	{
		GenerateFish(FishType);
	}
}

FVector AWaterManager::GetRandomPointInWater(int Counter)
{
	FVector point = FVector(FMath::RandRange(min.X, max.X), FMath::RandRange(min.Y, max.Y), 0);
	if (IsPointInWater(point))
	{
		return point;
	}

	if(Counter > 100)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx error: too many iterations"));
		return center;
	}
	return GetRandomPointInWater(Counter + 1);
}

bool AWaterManager::IsPointInWater(FVector point) const
{
	bool result = FGeomTools2D::IsPointInPolygon(FVector2D(point), WaterBounds);
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



