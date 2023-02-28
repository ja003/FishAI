// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NextLevelTrigger.generated.h"

/**
 * Trigger area to start a next level
 */
UCLASS()
class FISHAI_API ANextLevelTrigger : public AActor
{
	GENERATED_BODY()

public:
	
	ANextLevelTrigger();

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerArea;

};
