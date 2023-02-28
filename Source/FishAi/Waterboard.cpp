// Fill out your copyright notice in the Description page of Project Settings.


#include "Waterboard.h"

#include "Components/BoxComponent.h"


AWaterboard::AWaterboard()
{
	BoardCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoardCollider"));
	RootComponent = BoardCollider;
	BoardCollider->SetSimulatePhysics(true);
	BoardCollider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractArea = CreateDefaultSubobject<UBoxComponent>("InteractArea");
	InteractArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InteractArea->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}

void AWaterboard::SetInput(float X, float Y)
{
	BoardCollider->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	BoardCollider->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	
	FVector forwardVector = GetActorForwardVector();
	forwardVector.Z = 0;
	SetActorLocation(GetActorLocation() + forwardVector * Y * Data->ForwardSpeed);
	AddActorWorldRotation(FRotator(0, X * Data->TurnSpeed, 0));
}
