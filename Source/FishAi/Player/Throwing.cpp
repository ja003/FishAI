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

	if (LastActiveObject != EThrowableObject::None)
		Prediction->UpdateValues(GetThrowStart()->GetComponentLocation(), GetThrowStart()->GetForwardVector() * ThrowPower, ignoredActors);
}

// Sets default values for this component's properties
UThrowing::UThrowing()
{
	PrimaryComponentTick.bCanEverTick = true;
}


USceneComponent* UThrowing::GetThrowStart()
{
	switch (LastActiveObject) { 
	case EThrowableObject::Rock: return ThrowStartRock;
	case EThrowableObject::Bait: return ThrowStartBait;
	case EThrowableObject::Grenade: return ThrowStartGrenade;
	}

	checkNoEntry()
	return ThrowStartRock;
}

// Called when the game starts
void UThrowing::BeginPlay()
{
	Super::BeginPlay();

	//Prediction = Cast<AThrowPrediction>(GetWorld()->SpawnActor(AThrowPrediction::StaticClass()));
	
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
	//UE_LOG(LogTemp, Log, TEXT("xxx newYaw = %f"), newYaw);
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
		UE_LOG(LogTemp, Log, TEXT("xxx no spawned object to throw"));
		return;
	}

	SpawnedObject->Throw(GetThrowStart()->GetForwardVector() * ThrowPower);
	Inventory->RemoveItem(SpawnedObject->GetType());

	SpawnedObject = nullptr;
	Prediction->SetEnabled(false);
}

void UThrowing::SetActiveObject(EThrowableObject ObjectType)
{
	if(bIsThrowing) return;
	
	//UE_LOG(LogTemp, Log, TEXT("xxx SetActiveObject = %d"), (int)ObjectType);

	if (!Inventory->HasItem(ObjectType))
	{
		UE_LOG(LogTemp, Log, TEXT("xxx item %d not in inventory"), (int)ObjectType);
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
	//spawnTransform.SetRotation(FRotator::MakeFromEuler(Data->Throwable[ObjectType]->SpawnRotation).Quaternion());

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

	//UE_LOG(LogTemp, Log, TEXT("xxx DeselectObjects"));

	if(SpawnedObject != nullptr)
	{
		SpawnedObject->Destroy();
		SpawnedObject = nullptr;
	}
	
	Prediction->SetEnabled(false);
}

bool UThrowing::CanStartThrowing()
{
	return !bIsThrowing && SpawnedObject != nullptr;
}

