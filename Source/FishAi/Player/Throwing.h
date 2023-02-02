// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishAi/PlayerObjects/ThrowableObject.h"
#include "Throwing.generated.h"


enum class UEThrowableObject : uint8;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UThrowing : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UThrowing();

	USkeletalMeshComponent* SkeletalMesh;
	
	const FName RightHand_SocketName = "RightHand";

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//TList<UEThrowableObject> ObjectsToThrow;

	public:

	UFUNCTION(BlueprintCallable)
	void SetActiveObject(int Index);

	UFUNCTION(BlueprintCallable)
	void DeselectObjects();

	UFUNCTION(BlueprintImplementableEvent)
	void SetPredictionEnabled(bool bEnabled);

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AThrowableObject>> ThrowableObjects;

	AThrowableObject* SpawnedObject;

};
