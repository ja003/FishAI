// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ThrowableObject.generated.h"

class USphereComponent;

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

	UFUNCTION(BlueprintCallable)
	void SetVelocity(FVector Velocity);
	
public:		// Senses

	UFUNCTION(BlueprintImplementableEvent, Category = "Senses")
	void ReportNoise();	
	
	virtual EStimuliObject GetStimuliType() override { return None; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAIPerceptionStimuliSourceComponent* StimuliSource;

};
