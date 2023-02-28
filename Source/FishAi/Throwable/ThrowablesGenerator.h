// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EThrowableObjectType.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ThrowablesGenerator.generated.h"

class UThrowableGeneratorData;

/**
 * Gives an object to a player on interaction
 */
UCLASS()
class FISHAI_API AThrowablesGenerator : public AActor
{
	GENERATED_BODY()

public:

	AThrowablesGenerator();

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);

	virtual void BeginPlay() override;

	// Sets opacity to mesh showing the state of cooldown.
	// 0 = ready, 1 = not-ready
	void SetStateOpacity(float opacity);

	virtual void Tick(float DeltaSeconds) override;
	

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StateMesh;
	
	int64 LastTimeItemAdded;
	
	float GetRemainingCooldown();

public:

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* Material;
	
	UPROPERTY(EditDefaultsOnly)
	EThrowableObjectType ObjectType;

	UPROPERTY(EditDefaultsOnly)
	UThrowableGeneratorData* Data;

	bool bIsDisabled;
	
	void Disable();


private:
	
	int Cooldown;
	
};
