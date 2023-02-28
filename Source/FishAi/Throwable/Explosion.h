// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Explosion.generated.h"

UCLASS()
class FISHAI_API AExplosion : public AActor
{
	GENERATED_BODY()

public:
	
	AExplosion();

protected:
	
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* Particles;

	UPROPERTY(EditDefaultsOnly)
	URadialForceComponent* RadialForce;
	
};
