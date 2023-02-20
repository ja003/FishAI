// Fill out your copyright notice in the Description page of Project Settings.


#include "NavModifier.h"

//#include "NavModifierComponent.h"


// Sets default values
ANavModifier::ANavModifier()
{
	//NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT(""));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	RootComponent = Collider;

	Collider->bDynamicObstacle = true;
	//Collider->RegisterComponent();
}

void ANavModifier::GenerateBetweenPoints(FVector2D PointA, FVector2D PointB)
{
	FVector2D partDir = PointB - PointA;
	float partLength = partDir.Length();
		
	FVector2D boundPartCenter2D = (PointA + PointB) / 2;
	FVector boundPartCenter = FVector(boundPartCenter2D.X, boundPartCenter2D.Y, 0);

	FVector partDir3 = FVector(partDir.X, partDir.Y, 0);
	//ANavModifier* navMod = GetWorld()->SpawnActor<ANavModifier>(NavModifierBP, boundPartCenter, partDir3.Rotation());
	SetActorLocation(boundPartCenter);
	SetActorRotation(partDir3.Rotation());

	//UE_LOG(LogTemp, Log, TEXT("xxx spawn modifier"));
	//DrawDebugSphere(GWorld, boundPartCenter, 50, 10, FColor::Blue, false, 5);

	//navMod->NavModifier->FailsafeExtent = FVector::OneVector * 500;
	Collider->SetBoxExtent(FVector(partLength / 2, 150, 1000));
}
