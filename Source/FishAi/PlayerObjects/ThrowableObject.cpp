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
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource");
	// todo: doesnt work, only when configured in BP
	// StimuliSource->bAutoRegister = true;
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
		SetVelocity(bDebug_Velocity);

	NoiseReporter = Cast<ANoiseReporter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANoiseReporter::StaticClass()));
	check(NoiseReporter)

	Data = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	check(Data)
}

void AThrowableObject::SetVelocity(FVector Velocity)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	ProjectileMovement->bSimulationEnabled = true;
	
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//SphereCollider->SetSimulatePhysics(true);
	//SphereCollider->SetEnableGravity(true);

	//UE_LOG(LogTemp, Log, TEXT("xxx Velocity = %s"), *Velocity.ToString());
	ProjectileMovement->Velocity = Velocity;

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

	int range = Data->Throwable->NoiseRange[GetType()];
	NoiseReporter->ReportNoise(nullptr, GetActorLocation(), nullptr, 1, range, GetTag());

	DrawDebugSphere(GWorld, GetActorLocation(), range, 10, FColor::Yellow, false, .1f);
}

EThrowableObject AThrowableObject::GetType()
{
	checkNoEntry();
	return EThrowableObject::None;
}

