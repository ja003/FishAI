﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion.h"
#include "ThrowableObject.h"
#include "UObject/Object.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class FISHAI_API AGrenade : public AThrowableObject
{
	GENERATED_BODY()

public:
	virtual void OnEnteredWater() override;

	void Explode();

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	TSubclassOf<AExplosion> ExplosionBP = AExplosion::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	int ExplosionStrength = 100000;

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	float Radius;
};
