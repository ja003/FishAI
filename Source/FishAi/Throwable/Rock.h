// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableObject.h"
#include "FishAi/Constants.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"

/**
 * Scares away fishes
 */
UCLASS()
class FISHAI_API ARock : public AThrowableObject
{
	GENERATED_BODY()

public:
	
	ARock();

	virtual EStimuliObject GetStimuliType() override { return Rock; }

	virtual EThrowableObjectType GetType() override { return EThrowableObjectType::Rock; };

	virtual FName GetTag() override { return Tag_Rock; }
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Sound)
	USoundBase* Sound_HitWater;

};
