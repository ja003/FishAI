// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ThrowableObject.generated.h"

class USphereComponent;

// required:
//	add Buoyancy component to new object in BP and call OnEnteredWater from
//	the event
UCLASS()
class FISHAI_API AThrowableObject : public AActor, public IStimuliSource
{
	GENERATED_BODY()

public:
	
	AThrowableObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UProjectileMovementComponent* ProjectileMovement;

	//todo: how to include this???
	//UBuoyancyComponent Buoyancy;

	UFUNCTION(BlueprintCallable)
	void SetVelocity(FVector Velocity);

	// this has to be called from Buoyancy component event in BP 
	UFUNCTION(BlueprintCallable)
	virtual void OnEnteredWater();
	
public:		// Senses

	UFUNCTION(BlueprintImplementableEvent, Category = "Senses")
	void ReportNoise();	
	
	virtual EStimuliObject GetStimuliType() override { return None; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAIPerceptionStimuliSourceComponent* StimuliSource;

};
