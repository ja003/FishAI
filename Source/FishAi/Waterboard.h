// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Data/WaterboardData.h"
#include "GameFramework/Actor.h"
#include "Waterboard.generated.h"

UCLASS()
class FISHAI_API AWaterboard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterboard();

public:

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoardCollider;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StateMesh;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* InteractArea;
	
	UPROPERTY(EditDefaultsOnly)
	UWaterboardData* Data;

	UFUNCTION(BlueprintCallable)
	void SetInput(float X, float Y);
};
