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

	virtual void BeginPlay() override;
	void SetStateOpacity(float opacity);

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StateMesh;
	
	int64 LastTimeItemAdded;
	
	float GetRemainingCooldown();

public:

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* Material;
	
	UPROPERTY(EditDefaultsOnly)
	EThrowableObject ObjectType;

	UPROPERTY(EditAnywhere)
	int Cooldown;
	
};
