// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwing.h"



// Sets default values for this component's properties
UThrowing::UThrowing()
{
}


// Called when the game starts
void UThrowing::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UThrowing::SetActiveObject(int Index)
{
	UE_LOG(LogTemp, Log, TEXT("xxx SetActiveObject = %d"), Index);

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform spawnTransform = SkeletalMesh->GetSocketTransform(RightHand_SocketName);
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(ThrowableObjects[0], spawnTransform, SpawnParams);
	SpawnedObject = Cast<AThrowableObject>(spawnActor);

	SpawnedObject->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, RightHand_SocketName);

	SetPredictionEnabled(true);
}

void UThrowing::DeselectObjects()
{
	UE_LOG(LogTemp, Log, TEXT("xxx DeselectObjects"));

	SetPredictionEnabled(false);
}

