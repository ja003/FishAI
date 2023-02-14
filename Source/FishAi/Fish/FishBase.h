// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "FishBase.generated.h"

class AWaterManager;
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

	virtual void Tick(float DeltaSeconds) override;

protected:

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg,
		const FHitResult& HitResult) {}

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult) {}

	UFUNCTION()
	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) {}

public:

	void OnKilledByGrenade(FVector ExplosionForce);

	bool IsDead;
	
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
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	USphereComponent* MouthCollider;

	// Default value measured by observation
	UPROPERTY(EditAnywhere)
	FVector AfterDeathTorque = FVector(3000, 20000, 10000);

public: // Variables

	UPROPERTY(EditAnywhere)
	AWaterManager* Water;


};
