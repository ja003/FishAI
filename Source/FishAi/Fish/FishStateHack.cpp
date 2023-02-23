// Fill out your copyright notice in the Description page of Project Settings.


#include "FishStateHack.h"

#include "FishBase.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Managers/WaterManager.h"


// Sets default values for this component's properties
UFishStateHack::UFishStateHack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


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
	bool isStucked = IsStuck();
	if(isStucked)
		stuckCounter++;
	else
		stuckCounter = 0;

	lastLocation = body->GetComponentLocation();

	if(stuckCounter > 3)
		UnStuck();
}

bool UFishStateHack::IsStuck()
{
	float distance = FVector::Distance(body->GetComponentLocation(), lastLocation);
	//UE_LOG(LogTemp, Log, TEXT("xxx distance = %f"), distance);
	return distance < MinMoveDistance;
}

void UFishStateHack::UnStuck()
{
	FVector location = GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Log, TEXT("xxx UnStuck from = %s"), *location.ToString());
	Water->UpdateInWaterTarget(location);
	//UE_LOG(LogTemp, Log, TEXT("xxx UnStuck to = %s"), *location.ToString());
	//DrawDebugSphere(GWorld, location, 5, 50, FColor::Blue, false, 5);
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

	UE_LOG(LogTemp, Log, TEXT("xxx error: fish not on navmesh -> respawn"));
	Water->GenerateFish(fish->Type);
	fish->Die();
}

