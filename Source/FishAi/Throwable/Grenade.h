// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion.h"
#include "ThrowableObject.h"
#include "FishAi/Constants.h"
#include "UObject/Object.h"
#include "Grenade.generated.h"

/**
 * Explodes after hitting ground or watter and kills fishes
 */
UCLASS()
class FISHAI_API AGrenade : public AThrowableObject
{
	GENERATED_BODY()

	AGrenade();

public:
	virtual void OnEnteredWater() override;

	virtual void OnHitGround() override;

	virtual EThrowableObjectType GetType() override { return EThrowableObjectType::Grenade; }
	
	virtual FName GetTag() override { return Tag_Grenade; }

	void Explode();

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	TSubclassOf<AExplosion> ExplosionBP = AExplosion::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	int ExplosionStrength = 100000;

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	float Radius;
};
