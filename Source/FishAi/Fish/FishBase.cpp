// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

#include "AIController.h"
#include "FishStateHack.h"
#include "BrainComponent.h"
#include "EFishState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/Managers/ScoreManager.h"
#include "FishAi/StimuliObject.h"
#include "FishAi/Managers/WaterManager.h"
#include "FishAi/Data/DataManager.h"
#include "FishAi/Managers/NoiseReporter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AFishBase::AFishBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// MESH & COLLISIONS
	
	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("body"));
	bodyMesh->AddLocalRotation(FRotator(0,-90,0));
	bodyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	bodyMesh->SetCanEverAffectNavigation(false);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    
    MouthCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Mouth"));
    MouthCollider->AttachToComponent(bodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

    MouthCollider->OnComponentBeginOverlap.AddDynamic(this, &AFishBase::OnMouthBeginOverlap);
    MouthCollider->SetCollisionObjectType(COLLISION_FISH);
    MouthCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    MouthCollider->SetCollisionResponseToChannel(COLLISION_FISH, ECR_Overlap);
    MouthCollider->SetCollisionResponseToChannel(COLLISION_THROWABLE_OBJECT, ECR_Overlap);
    MouthCollider->SetCollisionResponseToChannel(COLLISION_EXPLOSION, ECR_Block);

	// AI
	
	// needs to be set otherwise AI controller doesnt initiate
	// when spawned from code
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AFishBase::OnTargetPerceptionUpdated);

	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;

	AISenseConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	AISenseConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigHearing->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigHearing->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	
	FishStateHack = CreateDefaultSubobject<UFishStateHack>("FishStateHack");	
}

void AFishBase::BeginPlay()
{
	Super::BeginPlay();

	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	check(blackboard)

	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Swimming);


	// fish actor has to be placed at the water bottom
	// (where navmesh is generated)
	// otherwise it wont move 
	FHitResult hit;
	if(GetWorld()->LineTraceSingleByObjectType(hit, GetActorLocation(), GetActorLocation() + FVector::DownVector * 1000, COLLISION_GROUND))
	{
		SetActorLocation(hit.Location);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("xxx error: fish not placed in water"));
	}

	Score = Cast<AScoreManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AScoreManager::StaticClass()));
	check(Score)

	NoiseReporter = Cast<ANoiseReporter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANoiseReporter::StaticClass()));
	check(NoiseReporter)

	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	check(DataManager)

	Data = DataManager->Fish[FishType];
	check(Data)

	AISenseConfigSight->SightRadius = Data->SightRadius;
	AISenseConfigSight->LoseSightRadius = Data->SightRadius * 2;
	AISenseConfigHearing->HearingRange = Data->HearRadius;

	AIPerceptionComponent->RequestStimuliListenerUpdate();

	GetCharacterMovement()->MaxSwimSpeed = Data->Speed;

	// debug: fish not spawned but placed in level
	if (Water == nullptr)
	{
		Water = Cast<AWaterManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterManager::StaticClass()));
		FishStateHack->Water = Water;
	}
}

void AFishBase::Init(AWaterManager* InWater)
{
	Water = InWater;
	
	if (Water == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("xxx error: water not assigned"));
		Water = Cast<AWaterManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterManager::StaticClass()));
	}
	check(Water)
	FishStateHack->Water = Water;
}

void AFishBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsDead)
	{
		// update body position so the fish is visually on the surface.
		// unfortunately this doesnt take waves into account
		bodyMesh->SetRelativeLocation(FVector(0,0, -GetActorLocation().Z));
	}
}

void AFishBase::OnKilledByGrenade(FVector ExplosionForce)
{
 	Cast<AAIController>(GetController())->BrainComponent->StopLogic("Death");

	// throw fish into air for nice visual effect
	bodyMesh->SetSimulatePhysics(true);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	bodyMesh->AddImpulse(ExplosionForce);
	int torque_offset = 1000;
	AfterDeathTorque += FVector(
		FMath::RandRange(-torque_offset, torque_offset),
		FMath::RandRange(-torque_offset, torque_offset),
		FMath::RandRange(-torque_offset, torque_offset));
	bodyMesh->AddTorqueInDegrees(AfterDeathTorque, NAME_None, true);

	Score->OnFishKilled(this);
	
	Die(2);
}

void AFishBase::OnEatenByFish()
{
	Die();
}

void AFishBase::Die(int DestroyDelay)
{
	IsDead = true;
	Water->OnFishDie(this);

	if (DestroyDelay <= 0)
		Destroy();
	else
		SetLifeSpan(DestroyDelay);
}

void AFishBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetActorLocation() + bodyMesh->GetRelativeLocation();
	OutRotation = GetActorRotation();
}

void AFishBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == this) return;
	
	if(Actor->GetClass()->ImplementsInterface(UStimuliSource::StaticClass()))
	{
		switch(Cast<IStimuliSource>(Actor)->GetStimuliType())
		{
		case Bait:
			OnBaitPerceptionUpdated(Actor, Stimulus);
			return;
		case Pike:
			OnPikePerceptionUpdated(Actor, Stimulus);
			return;

		case EdibleFish:
			OnEdibleFishPerceptionUpdated(Actor, Stimulus);
			return;

		case Rock:
			OnRockPerceptionUpdated(Actor, Stimulus);
			return;
		}
	}

	if (Stimulus.Tag == Tag_Rock)
	{
		OnRockPerceptionUpdated(Actor, Stimulus);
		return;
	}
	if (Stimulus.Tag == Tag_Bait)
	{
		OnBaitPerceptionUpdated(Actor, Stimulus);
		return;
	}
}

void AFishBase::SetFail(bool InValue)
{
	if(InValue)
	{
		blackboard->SetValueAsBool(FishBB_Fail, true);

		FTimerHandle UnusedHandle;
		FTimerDelegate TimerDel;
		
		TimerDel.BindUFunction(this, FName("SetFail"), false);
		
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, TimerDel, 0.1f, false);
	}
	else
	{
		blackboard->SetValueAsBool(FishBB_Fail, false);
	}
}

void AFishBase::RunawayFrom(FVector SourceLocation, int MaxDistance, EFishState NewState)
{
	// if we are running away from the same kind of source - force reevaluate
	// BT to update runaway location
	if (GetState() == NewState)
	{
		SetFail(true);
	}
	
	SetState(NewState);
	
	FVector dirAway = (GetActorLocation() - SourceLocation);
	dirAway.Z = 0;
	dirAway.Normalize();
	int runawayDistance = FMath::RandRange(MaxDistance / 2, MaxDistance);
	FVector runawayTarget = GetActorLocation() + dirAway * runawayDistance;
	FVector inWaterTarget = Water->GetClosestPointInWater(runawayTarget);
	
	if (FVector::Distance(inWaterTarget, GetActorLocation()) < 200)
	{
		FVector dirToRandom = Water->GetRandomPointInWater() - GetActorLocation();
		dirToRandom.Normalize();
		inWaterTarget = GetActorLocation() + dirToRandom * runawayDistance; 
	}
	
	Water->UpdateInWaterTarget(inWaterTarget);

	blackboard->SetValueAsVector(FishBB_Target, inWaterTarget);
}

void AFishBase::OnRockPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	// Actor is player character, not the rock!
	RunawayFrom(Stimulus.StimulusLocation, Data->RockRunawayDistance, EFishState::Rock);
}

void AFishBase::SetState(EFishState NewState)
{
	blackboard->SetValueAsEnum(FishBB_State, (int)NewState);
}

EFishState AFishBase::GetState()
{
	int state = blackboard->GetValueAsEnum(FishBB_State);
	return (EFishState)state;
}

