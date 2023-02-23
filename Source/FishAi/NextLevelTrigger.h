// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NextLevelTrigger.generated.h"

UCLASS()
class FISHAI_API ANextLevelTrigger : public AActor
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	ANextLevelTrigger();

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerArea;

};
