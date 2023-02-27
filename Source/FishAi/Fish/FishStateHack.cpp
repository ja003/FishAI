// Fill out your copyright notice in the Description page of Project Settings.


#include "FishStateHack.h"

#include "FishBase.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Managers/WaterManager.h"


// Called when the game starts
void UFishStateHack::BeginPlay()
{
	Super::BeginPlay();

	body = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	FTimerHandle CheckStuckHandle;
	GetWorld()->GetTimerManager().SetTimer(CheckStuckHandle, this, &UFishStateHack::CheckStuck, 1, true);

	FTimerHandle CheckInWaterHandle;
	GetWorld()->GetTimerManager().SetTimer(CheckInWaterHandle, this, &UFishStateHack::CheckInWater, 1, true);

	FTimerHandle CheckOnNavmeshHandle;
	GetWorld()->GetTimerManager().SetTimer(CheckOnNavmeshHandle, this, &UFishStateHack::CheckOnNavmesh, 1, true);
}

void UFishStateHack::CheckStuck()
{
	if (IsStuck())
		stuckCounter++;
	else
		stuckCounter = 0;

	lastLocation = body->GetComponentLocation();

	if (stuckCounter > 3)
		UnStuck();
}

bool UFishStateHack::IsStuck()
{
	float distance = FVector::Distance(body->GetComponentLocation(), lastLocation);
	return distance < MinMoveDistance;
}

void UFishStateHack::UnStuck()
{
	FVector location = GetOwner()->GetActorLocation();
	Water->UpdateInWaterTarget(location);
	GetOwner()->SetActorLocation(location);

	Cast<AFishBase>(GetOwner())->GetCapsuleComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	
	stuckCounter = 0;
}

void UFishStateHack::CheckInWater()
{
	// doesnt work very well...navagent doesnt move in straight line and sometimes
	// fish goes outside of bounds and it is evaluated as outside of water
	return;
	
	if (Water->IsPointInWater(GetOwner()->GetActorLocation()))
	{
		outsideWaterCounter = 0;
		return;
	}

	outsideWaterCounter++;
	UE_LOG(LogTemp, Log, TEXT("xxx warning: fish outside of water"));
	
	if (outsideWaterCounter > 6)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx error: fish is outside of water. Destroying"));
		Cast<AFishBase>(GetOwner())->Die();
	}
}

void UFishStateHack::CheckOnNavmesh()
{
	if (GetOwner()->GetActorLocation().Z > -9999)
		return;

	AFishBase* fish = Cast<AFishBase>(GetOwner());
	if (fish->IsDead)
		return;

	UE_LOG(LogTemp, Log, TEXT("xxx error: fish not on navmesh -> respawn"));
	Water->GenerateFish(fish->FishType);
	fish->Die();
}

