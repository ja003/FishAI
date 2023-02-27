// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Throwing.h"
#include "Waterboarding.h"
#include "Components/SphereComponent.h"
#include "FishAi/Data/PlayerData.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FISHAI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public: // water

	UFUNCTION(BlueprintCallable)
	void SetSwimming(bool IsSwimming);
	
private: // water
	
	UFUNCTION()
	void OnBallsBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);
	
	UFUNCTION()
	void OnBallsEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsInWater;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UThrowing* Throwing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWaterboarding* Waterboarding;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* BallsCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* ThrowStartRock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* ThrowStartBait;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* ThrowStartGrenade;

	UPROPERTY(EditDefaultsOnly)
	UPlayerData* Data;


	
	
};
