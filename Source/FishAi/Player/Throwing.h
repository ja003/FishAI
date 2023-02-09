// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishAi/PlayerObjects/ThrowableObject.h"
#include "FishAi/PlayerObjects/ThrowPrediction.h"
#include "Throwing.generated.h"


enum class UEThrowableObject : uint8;
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AThrowPrediction* Prediction;

	public:

	UFUNCTION(BlueprintCallable)
	void Throw();

	UFUNCTION(BlueprintCallable)
	void SetActiveObject(int Index);

	UFUNCTION(BlueprintCallable)
	void DeselectObjects();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AThrowableObject>> ThrowableObjects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AThrowableObject* SpawnedObject;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ThrowPower = 1000;

};
