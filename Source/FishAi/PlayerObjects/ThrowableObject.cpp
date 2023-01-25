// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableObject.h"

#include "Components/SphereComponent.h"
#include "FishAi/Constants.h"
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
	SphereCollider->SetCollisionObjectType(COLLISION_THROWABLE_OBJECT);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Block);
	SphereCollider->SetCollisionResponseToChannel(COLLISION_FISH, ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(COLLISION_WATER, ECR_Overlap);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
}

void AThrowableObject::SetVelocity(FVector Velocity)
{
	UE_LOG(LogTemp, Log, TEXT("xxx Velocity = %s"), *Velocity.ToString());
	ProjectileMovement->Velocity = Velocity;
}

