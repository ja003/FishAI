// Fill out your copyright notice in the Description page of Project Settings.


#include "AntiStackHack.h"

#include "FishBase.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Managers/WaterManager.h"


// Sets default values for this component's properties
UAntiStackHack::UAntiStackHack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


// Called when the game starts
void UAntiStackHack::BeginPlay()
{
	Super::BeginPlay();

	body = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UAntiStackHack::CheckStuck, 1, true);
	
}

void UAntiStackHack::CheckStuck()
{
	bool isStucked = IsStucked();
	if(isStucked)
		stuckedCounter++;
	else
		stuckedCounter = 0;

	lastLocation = body->GetComponentLocation();

	if(stuckedCounter > 3)
		UnStuck();
}

bool UAntiStackHack::IsStucked()
{
	float distance = FVector::Distance(body->GetComponentLocation(), lastLocation);
	//UE_LOG(LogTemp, Log, TEXT("xxx distance = %f"), distance);
	return distance < MinMoveDistance;
}

void UAntiStackHack::UnStuck()
{
	FVector location = GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Log, TEXT("xxx UnStuck from = %s"), *location.ToString());
	Water->UpdateInWaterTarget(location);
	//UE_LOG(LogTemp, Log, TEXT("xxx UnStuck to = %s"), *location.ToString());
	//DrawDebugSphere(GWorld, location, 5, 50, FColor::Blue, false, 5);
	GetOwner()->SetActorLocation(location);

	Cast<AFishBase>(GetOwner())->GetCapsuleComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	
	stuckedCounter = 0;
}

