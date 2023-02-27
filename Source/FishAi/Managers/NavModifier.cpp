// Fill out your copyright notice in the Description page of Project Settings.


#include "NavModifier.h"


// Sets default values
ANavModifier::ANavModifier()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	RootComponent = Collider;

	Collider->bDynamicObstacle = true;
}

void ANavModifier::GenerateBetweenPoints(FVector2D PointA, FVector2D PointB)
{
	FVector2D partDir = PointB - PointA;
	float partLength = partDir.Length();
		
	FVector2D boundPartCenter2D = (PointA + PointB) / 2;
	FVector boundPartCenter = FVector(boundPartCenter2D.X, boundPartCenter2D.Y, 0);

	FVector partDir3 = FVector(partDir.X, partDir.Y, 0);
	SetActorLocation(boundPartCenter);
	SetActorRotation(partDir3.Rotation());

	Collider->SetBoxExtent(FVector(partLength / 2, 150, 1000));
}
