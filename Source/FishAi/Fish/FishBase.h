// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AntiStackHack.h"
#include "EFish.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "FishBase.generated.h"

class ANoiseReporter;
enum class EFishState : uint8;
class ADataManager;
class AScoreManager;
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

	UPROPERTY(EditDefaultsOnly)
	int InWaterBodyOffset;

	UPROPERTY(EditDefaultsOnly)
	EFish Type = EFish::None;

	UFUNCTION(BlueprintCallable)
	void OnKilledByGrenade(FVector ExplosionForce);

	bool IsDead;
	
	void OnEatenByFish();

private:

	void Die(int DestroyDelay = 0);

protected: // AI

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	void RunawayFrom(FVector SourceLocation, int MaxDistance, EFishState NewState);

	virtual void OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus){}

	virtual void OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus){}

	virtual void OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) {}

	virtual void OnRockPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus);

	UPROPERTY()
	UBlackboardComponent* blackboard;

public:

	UFUNCTION(BlueprintCallable)
	virtual void SetState(EFishState NewState);
	
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

	UPROPERTY(EditDefaultsOnly)
	UAntiStackHack* AntiStackHack;

public: // Variables

	UPROPERTY(EditAnywhere)
	AWaterManager* Water;

	AScoreManager* Score;

	ADataManager* Data;

	ANoiseReporter* NoiseReporter; 



};
