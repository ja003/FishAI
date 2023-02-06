// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Throwing.h"
#include "Components/SphereComponent.h"
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


private: // water

	void SetSwimming(bool IsSwimming);
	
	UFUNCTION()
	void OnBallsBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);
	
	UFUNCTION()
	void OnBallsEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsInWater;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UThrowing* Throwing;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* BallsCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* ThrowStart;


	
	
};
