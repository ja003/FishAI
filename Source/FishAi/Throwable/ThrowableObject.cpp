// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableObject.h"

#include "Components/SphereComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/Data/DataManager.h"
#include "FishAi/Managers/NoiseReporter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


AThrowableObject::AThrowableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource");
	// todo: doesnt work, only when configured in BP
	StimuliSource->bAutoRegister = false;
	// StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("collider"));
	RootComponent = SphereCollider;
	SphereCollider->SetSimulatePhysics(false);
	SphereCollider->SetEnableGravity(false);
	SphereCollider->SetCollisionObjectType(COLLISION_THROWABLE_OBJECT);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Block);
	SphereCollider->SetCollisionResponseToChannel(
		ECC_Pawn
		//COLLISION_PLAYER
		, ECR_Ignore);
	SphereCollider->SetCollisionResponseToChannel(
		COLLISION_FISH, ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(
		COLLISION_WATER, ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(
		COLLISION_EXPLOSION, ECR_Block);
	//disable at start, enable when thrown
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollider->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	Mesh->SetCanEverAffectNavigation(false);
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}

	ProjectileMovement =  CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->bSimulationEnabled = false;

	//Tags.Add(GetTag());
}

void AThrowableObject::BeginPlay()
{
	Super::BeginPlay();

	if (bDebug_Throw)
		Throw(bDebug_Velocity);

	NoiseReporter = Cast<ANoiseReporter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANoiseReporter::StaticClass()));
	check(NoiseReporter)

	Data = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	check(Data)
}

void AThrowableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(ProjectileMovement->bSimulationEnabled)
		AddActorLocalRotation(Data->Throwable[GetType()]->TickRotation);
}

void AThrowableObject::Throw(FVector Velocity)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	ProjectileMovement->bSimulationEnabled = true;
	
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//SphereCollider->SetSimulatePhysics(true);
	//SphereCollider->SetEnableGravity(true);

	//UE_LOG(LogTemp, Log, TEXT("xxx Velocity = %s"), *Velocity.ToString());
	ProjectileMovement->Velocity = Velocity;

	//StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());

	// spawned object is scaled down. set original scale
	SetActorRelativeScale3D(FVector::OneVector);

	// randomize starting rotation
	FRotator throwStartRot = Data->Throwable[GetType()]->TickRotation;
	throwStartRot.Yaw = FMath::RandRange(-throwStartRot.Yaw, throwStartRot.Yaw);
	throwStartRot.Pitch = FMath::RandRange(-throwStartRot.Pitch, throwStartRot.Pitch);
	throwStartRot.Roll = FMath::RandRange(-throwStartRot.Roll, throwStartRot.Roll);
	SetActorRotation(throwStartRot);
}

FName AThrowableObject::GetTag()
{
	return FName("");
}

void AThrowableObject::OnEnteredWater()
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnEnteredWater"));
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_HitWater, GetActorLocation());

	ProjectileMovement->bSimulationEnabled = false;
	ProjectileMovement->Velocity = FVector::ZeroVector;

	SphereCollider->SetSimulatePhysics(true);
	SphereCollider->SetEnableGravity(true);

	int range = Data->Throwable[GetType()]->NoiseRange;
	//todo: play sound at water bottom where fishes have ears?
	NoiseReporter->ReportNoise(nullptr, GetActorLocation(), nullptr, 1, range, GetTag());

	DrawDebugSphere(GWorld, GetActorLocation(), range, 10, FColor::Yellow, false, .1f);

	StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliSource->RegisterWithPerceptionSystem();

}

EThrowableObject AThrowableObject::GetType()
{
	checkNoEntry();
	return EThrowableObject::None;
}

