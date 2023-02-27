// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FishStateHack.h"
#include "EFish.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "FishBase.generated.h"

class UFishData;
class ANoiseReporter;
enum class EFishState : uint8;
class ADataManager;
class AScoreManager;
class AWaterManager;

/*
 * Base class for fishes
 */
UCLASS()
class FISHAI_API AFishBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	AFishBase();

protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:

	virtual void Init(AWaterManager* InWater);

protected: // Collisions

	UFUNCTION()
	virtual void OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult) {}

public:

	UPROPERTY(EditDefaultsOnly)
	EFish FishType = EFish::None;

public: // Death

	UFUNCTION(BlueprintCallable)
	void OnKilledByGrenade(FVector ExplosionForce);

	bool IsDead;
	
	void OnEatenByFish();

	void Die(int DestroyDelay = 0);
	
	// Torque applied after kill by grenade (for better effect) 
	// Default value measured by observation
	UPROPERTY(EditAnywhere)
	FVector AfterDeathTorque = FVector(3000, 20000, 10000);

protected: // AI

	// The physical body is positioned on the bottom of a water but
	// eyes have to be on the surface so they can see objects in water
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Forces BT to reevaluate
	UFUNCTION()
	void SetFail(bool InValue);

	// Sets NewState and move Target to run away from SourceLocation
	void RunawayFrom(FVector SourceLocation, int MaxDistance, EFishState NewState);

	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus){}

	virtual void OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus){}

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) {}

	virtual void OnRockPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus);

	UPROPERTY()
	UBlackboardComponent* blackboard;

	TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight = nullptr;
	TObjectPtr<class UAISenseConfig_Hearing> AISenseConfigHearing = nullptr;

public:

	UFUNCTION(BlueprintCallable)	
	virtual void SetState(EFishState NewState);

	EFishState GetState();
	
protected: // Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* bodyMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	USphereComponent* MouthCollider;

	UPROPERTY(EditDefaultsOnly)
	UFishStateHack* FishStateHack;

protected:
	
	UFishData* Data;
	
public: // Managers

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AWaterManager* Water;

	AScoreManager* Score;

	ADataManager* DataManager;

	ANoiseReporter* NoiseReporter; 

};
