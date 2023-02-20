// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EThrowableObject.h"
#include "ThrowableObject.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ThrowableObject.generated.h"

class ADataManager;
class ANoiseReporter;
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

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UProjectileMovementComponent* ProjectileMovement;

	//todo: how to include this???
	//UBuoyancyComponent Buoyancy;

	UFUNCTION(BlueprintCallable)
	void Throw(FVector Velocity);
	virtual FName GetTag();

	UPROPERTY(EditAnywhere)
	bool bDebug_Throw;

	UPROPERTY(EditAnywhere)
	FVector bDebug_Velocity;
	
	// this has to be called from Buoyancy component event in BP 
	UFUNCTION(BlueprintCallable)
	virtual void OnEnteredWater();

	virtual EThrowableObject GetType();

	ADataManager* Data;
	
public:		// Senses

	UFUNCTION(BlueprintImplementableEvent, Category = "Senses")
	void ReportNoise();	
	
	virtual EStimuliObject GetStimuliType() override { return None; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	ANoiseReporter* NoiseReporter; 


};
