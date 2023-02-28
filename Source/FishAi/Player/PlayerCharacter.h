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

	APlayerCharacter();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

public: // water

	UFUNCTION(BlueprintCallable)
	void SetSwimming(bool IsSwimming);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsInWater;
	
private: // water
	
	UFUNCTION()
	void OnBallsBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);
	
	UFUNCTION()
	void OnBallsEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

public: // components	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UThrowing* Throwing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWaterboarding* Waterboarding;

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
