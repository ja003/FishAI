// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableObject.h"
#include "FishAi/Constants.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"

UCLASS()
class FISHAI_API ARock : public AThrowableObject
{
	GENERATED_BODY()

public:
	
	ARock();

	virtual EStimuliObject GetStimuliType() override { return Rock; }

	virtual EThrowableObject GetType() override { return EThrowableObject::Rock; };

	virtual FName GetTag() override { return Tag_Rock; }
	
private:

	bool bHasHitGround;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult);

	void DestroyRock();

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Sound)
	USoundBase* Sound_HitWater;

};
