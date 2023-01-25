// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableObject.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"

UCLASS()
class FISHAI_API ARock : public AThrowableObject
{
	GENERATED_BODY()

public:
	
	ARock();
	void OnHitWater();

	virtual EStimuliObject GetStimuliType() override { return Rock; }


private:

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg,
		const FHitResult& HitResult);


	bool bHasHitGround;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult);

	void DestroyRock();

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Sound)
	USoundBase* Sound_HitWater;

};
