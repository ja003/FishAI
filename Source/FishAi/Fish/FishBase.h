// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "FishBase.generated.h"

UCLASS()
class FISHAI_API AFishBase : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AFishBase();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg,
		const FHitResult& HitResult) {}

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult) {}

public:

	void Die();

protected: // AI

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus){}

	virtual void OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus){}

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) {}

	virtual void OnRockPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus);

	UPROPERTY()
	UBlackboardComponent* blackboard;
	
protected: // Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* bodyMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UAIPerceptionComponent* AIPerceptionComponent ;

public: // Variables


};
