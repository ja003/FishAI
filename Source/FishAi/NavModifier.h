// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NavModifier.generated.h"

UCLASS()
class FISHAI_API ANavModifier : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANavModifier();

	// UPROPERTY(EditAnywhere)
	// UNavModifierComponent* NavModifier;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
};
