// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwing.h"

#include "FishAi/Data/DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void UThrowing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());
	ignoredActors.Add(SpawnedObject);

	if (LastActiveObject != EThrowableObjectType::None)
		Prediction->UpdateValues(GetThrowStart()->GetComponentLocation(), GetThrowStart()->GetForwardVector() * ThrowPower, ignoredActors);
}

UThrowing::UThrowing()
{
	PrimaryComponentTick.bCanEverTick = true;
}


USceneComponent* UThrowing::GetThrowStart()
{
	switch (LastActiveObject) { 
	case EThrowableObjectType::Rock: return ThrowStartRock;
	case EThrowableObjectType::Bait: return ThrowStartBait;
	case EThrowableObjectType::Grenade: return ThrowStartGrenade;
	}

	checkNoEntry()
	return ThrowStartRock;
}

void UThrowing::BeginPlay()
{
	Super::BeginPlay();
	
	Prediction = Cast<AThrowPrediction>(UGameplayStatics::GetActorOfClass(GetWorld(), AThrowPrediction::StaticClass()));

	Data = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));

	Inventory = GetOwner()->FindComponentByClass<UInventory>();
	
	Capsule = GetOwner()->FindComponentByClass<UCapsuleComponent>();
	
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();	
}

void UThrowing::RotateCharacterToFaceCamera()
{
	FVector playerLoc = GetOwner()->GetActorLocation();
	float newYaw = UKismetMathLibrary::FindLookAtRotation(playerLoc, playerLoc + Camera->GetForwardVector()).Yaw;
	Capsule->SetWorldRotation(FRotator(0,newYaw,0));
}

void UThrowing::SetIsThrowing(bool bValue)
{
	bIsThrowing = bValue;

	if (!bIsThrowing)
	{
		SetActiveObject(LastActiveObject);
	}
}

void UThrowing::Throw()
{
	if(SpawnedObject == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("xxx error: no spawned object to throw"));
		return;
	}

	SpawnedObject->Throw(GetThrowStart()->GetForwardVector() * ThrowPower);
	Inventory->RemoveItem(SpawnedObject->GetType());

	SpawnedObject = nullptr;
	Prediction->SetEnabled(false);
}

void UThrowing::SetActiveObject(EThrowableObjectType ObjectType)
{
	if(bIsThrowing) return;
	
	if (!Inventory->HasItem(ObjectType))
	{
		DeselectObjects();
		return;
	}

	if (SpawnedObject != nullptr)
	{
		SpawnedObject->Destroy();
	}

	LastActiveObject = ObjectType;
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform spawnTransform = SkeletalMesh->GetSocketTransform(RightHand_SocketName);

	check(Inventory->ThrowableObjectsBP[ObjectType])
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(Inventory->ThrowableObjectsBP[ObjectType], spawnTransform, SpawnParams);
	SpawnedObject = Cast<AThrowableObject>(spawnActor);

	SpawnedObject->ProjectileMovement->bSimulationEnabled = false;
	SpawnedObject->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, RightHand_SocketName);

	// adjust object to fit into hand
	SpawnedObject->SetActorRelativeTransform(Data->Throwable[ObjectType]->SpawnOffset);
	
	Prediction->SetEnabled(true);

	RotateCharacterToFaceCamera();
}

void UThrowing::DeselectObjects()
{
	if(bIsThrowing) return;

	if(SpawnedObject != nullptr)
	{
		SpawnedObject->Destroy();
		SpawnedObject = nullptr;
	}
	
	Prediction->SetEnabled(false);

	ThrowPower = 1000;
}

bool UThrowing::CanStartThrowing()
{
	return !bIsThrowing && SpawnedObject != nullptr;
}

