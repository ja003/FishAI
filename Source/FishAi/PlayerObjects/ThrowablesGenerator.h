// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EThrowableObject.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ThrowablesGenerator.generated.h"

UCLASS()
class FISHAI_API AThrowablesGenerator : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AThrowablesGenerator();


private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	
	int64 LastTimeItemAdded;

public:
	
	UPROPERTY(EditAnywhere)
	EThrowableObject ObjectType;


	UPROPERTY(EditAnywhere)
	int Cooldown;
	
};
