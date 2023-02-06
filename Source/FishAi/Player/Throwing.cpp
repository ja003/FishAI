// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwing.h"

#include "Kismet/GameplayStatics.h"


void UThrowing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());
	ignoredActors.Add(SpawnedObject);
	Prediction->UpdateValues(ThrowStart->GetComponentLocation(), ThrowStart->GetForwardVector() * ThrowPower, ignoredActors);
}

// Sets default values for this component's properties
UThrowing::UThrowing()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UThrowing::BeginPlay()
{
	Super::BeginPlay();

	//Prediction = Cast<AThrowPrediction>(GetWorld()->SpawnActor(AThrowPrediction::StaticClass()));
	
	Prediction = Cast<AThrowPrediction>(UGameplayStatics::GetActorOfClass(GetWorld(), AThrowPrediction::StaticClass()));
}

void UThrowing::Throw()
{
	if(SpawnedObject == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx no spawned object to throw"));
		return;
	}

	SpawnedObject->SetVelocity(ThrowStart->GetForwardVector() * ThrowPower);
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

	Prediction->SetEnabled(true);
}

void UThrowing::DeselectObjects()
{
	UE_LOG(LogTemp, Log, TEXT("xxx DeselectObjects"));

	if(SpawnedObject != nullptr)
	{
		SpawnedObject->Destroy();
		SpawnedObject = nullptr;
	}
	
	Prediction->SetEnabled(false);
}

