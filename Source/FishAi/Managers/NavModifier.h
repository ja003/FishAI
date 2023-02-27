// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NavModifier.generated.h"

/**
 * Obstacle for navagents generated at lake shores
 */
UCLASS()
class FISHAI_API ANavModifier : public AActor
{
	GENERATED_BODY()

public:

	ANavModifier();

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	
	void GenerateBetweenPoints(FVector2D PointA, FVector2D PointB);
};
