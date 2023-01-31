// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetObject.h"

#include "Components/SphereComponent.h"	


// Sets default values
ATargetObject::ATargetObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USphereComponent* sphere = CreateDefaultSubobject<USphereComponent>("body");
	sphere->SetSimulatePhysics(false);
	sphere->SetGenerateOverlapEvents(true);
	sphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	RootComponent = sphere;
}

// Called when the game starts or when spawned
void ATargetObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATargetObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

