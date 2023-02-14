// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/PlayerObjects/ThrowableObject.h"
#include "FishAi/PlayerObjects/ThrowPrediction.h"
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
	USceneComponent* ThrowStart;

	const FName RightHand_SocketName = "RightHand";

private:
	
	EThrowableObject LastActiveObject;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AThrowPrediction* Prediction;

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

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AThrowableObject* SpawnedObject;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ThrowPower = 1000;

};
