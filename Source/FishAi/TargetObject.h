// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TargetObject.generated.h"

UCLASS()
class FISHAI_API ATargetObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATargetObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
