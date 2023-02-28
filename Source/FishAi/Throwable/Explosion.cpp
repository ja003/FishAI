// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"

#include "PhysicsEngine/RadialForceComponent.h"


AExplosion::AExplosion()
{
	PrimaryActorTick.bCanEverTick = false;

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particles"));
	Particles->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("X"));
	RadialForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3);	
}
