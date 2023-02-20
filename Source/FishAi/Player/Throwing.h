// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Throwable/ThrowableObject.h"
#include "FishAi/Throwable/ThrowPrediction.h"
#include "Throwing.generated.h"


enum class EThrowableObject : uint8;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UThrowing : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Sets default values for this component's properties
	UThrowing();

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY()
	USceneComponent* ThrowStartRock;
	UPROPERTY()
	USceneComponent* ThrowStartBait;
	UPROPERTY()
	USceneComponent* ThrowStartGrenade;

	const FName RightHand_SocketName = "RightHandIndex1";

private:
	
	EThrowableObject LastActiveObject;

	USceneComponent* GetThrowStart();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AThrowPrediction* Prediction;
	
	ADataManager* Data;

	UInventory* Inventory;

	UCapsuleComponent* Capsule;
	
	UCameraComponent* Camera;

	void RotateCharacterToFaceCamera();
	
public:

	UFUNCTION(BlueprintCallable)
	void SetIsThrowing(bool bValue);

	UPROPERTY(BlueprintReadOnly)
	bool bIsThrowing;
	
	UFUNCTION(BlueprintCallable)
	void Throw();

	UFUNCTION(BlueprintCallable)
	void SetActiveObject(EThrowableObject ObjectType);

	UFUNCTION(BlueprintCallable)
	void DeselectObjects();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStartThrowing();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AThrowableObject* SpawnedObject;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ThrowPower = 1000;

};
