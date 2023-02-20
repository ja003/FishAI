// Fill out your copyright notice in the Description page of Project Settings.


#include "Waterboard.h"

#include "Components/BoxComponent.h"


// Sets default values
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
	
	// UE_LOG(LogTemp, Log, TEXT("xxx SetInput X = %f"), X);
	// UE_LOG(LogTemp, Log, TEXT("xxx SetInput Y = %f"), Y);
	FVector forwardVector = GetActorForwardVector();
	forwardVector.Z = 0;
	//BoardCollider->AddForce(GetActorForwardVector() * X * ForwardSpeed);
	SetActorLocation(GetActorLocation() + forwardVector * Y * ForwardSpeed);

	AddActorWorldRotation(FRotator(0, X * TurnSpeed, 0));
}
