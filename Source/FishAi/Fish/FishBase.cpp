// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

#include "AIController.h"
#include "AntiStackHack.h"
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

// Sets default values
AFishBase::AFishBase()
{
	PrimaryActorTick.bCanEverTick = true;
		
	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("body"));
	bodyMesh->AddLocalRotation(FRotator(0,-90,0));
	bodyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	bodyMesh->SetCanEverAffectNavigation(false);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	// AISenseConfigSight->SightRadius = Data->SightRadius;
	// AISenseConfigSight->LoseSightRadius = Data->SightRadius * 2;
	// AISenseConfigHearing->HearingRange = Data->HearRadius;
	//
	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	// GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFishBase::OnComponentBeginOverlap);
	//
	// GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFishBase::OnComponentHit);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	MouthCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Mouth"));
	MouthCollider->AttachToComponent(bodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	MouthCollider->OnComponentBeginOverlap.AddDynamic(this, &AFishBase::OnMouthBeginOverlap);
	MouthCollider->SetCollisionObjectType(COLLISION_FISH);
	MouthCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	MouthCollider->SetCollisionResponseToChannel(COLLISION_FISH, ECR_Overlap);
	MouthCollider->SetCollisionResponseToChannel(COLLISION_THROWABLE_OBJECT, ECR_Overlap);
	MouthCollider->SetCollisionResponseToChannel(COLLISION_EXPLOSION, ECR_Block);

	AntiStackHack = CreateDefaultSubobject<UAntiStackHack>("AntiStuckHack");	

}

// Called when the game starts or when spawned
void AFishBase::BeginPlay()
{
	Super::BeginPlay();

	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	check(blackboard)

	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Swimming);

	//Target = GetWorld()->SpawnActor<ATargetObject>();

	// fish actor has to be placed at the water bottom (where navmesh is generated)
	// otherwise it wont mmove 
	FHitResult hit;
	if(GetWorld()->LineTraceSingleByObjectType(hit, GetActorLocation(), GetActorLocation() + FVector::DownVector * 1000, COLLISION_GROUND))
	{
		SetActorLocation(hit.Location);
		//UE_LOG(LogTemp, Log, TEXT("xxx new fish location = %s"), *hit.Location.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("xxx ERROR: fish not placed in water"));
	}

	if (Water == nullptr)
	{
		Water = Cast<AWaterManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaterManager::StaticClass()));
	}
	check(Water)
	AntiStackHack->Water = Water;

	Score = Cast<AScoreManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AScoreManager::StaticClass()));
	check(Score)

	NoiseReporter = Cast<ANoiseReporter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANoiseReporter::StaticClass()));
	check(NoiseReporter)

	DataManager = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	check(DataManager)

	Data = DataManager->Fish[Type];
	check(Data)

	AISenseConfigSight->SightRadius = Data->SightRadius;
	AISenseConfigSight->LoseSightRadius = Data->SightRadius * 2;
	AISenseConfigHearing->HearingRange = Data->HearRadius;

	AIPerceptionComponent->RequestStimuliListenerUpdate();

	GetCharacterMovement()->MaxSwimSpeed = Data->Speed;
}

void AFishBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsDead)
	{
		// this doesnt include waves:(
		//FVector waterSurfaceLoc = Water->GetWaterSurfaceLocation(GetActorLocation());
		// bodyMesh->SetWorldLocation(waterSurfaceLoc + FVector::UpVector * InWaterBodyOffset);
		bodyMesh->SetRelativeLocation(FVector(0,0, -GetActorLocation().Z));
	}
}

void AFishBase::OnKilledByGrenade(FVector ExplosionForce)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnKilledByPlayer"));

 	Cast<AAIController>(GetController())->BrainComponent->StopLogic("Death");
	
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

//todo: death by fish - animation
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
	
	//UE_LOG(LogTemp, Log, TEXT("xxx OnTargetPerceptionUpdated = %s"), *Actor->GetName());

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
	else if (Stimulus.Tag == Tag_Bait)
	{
		OnBaitPerceptionUpdated(Actor, Stimulus);
		return;
	}
}

void AFishBase::RunawayFrom(FVector SourceLocation, int MaxDistance, EFishState NewState)
{
	SetState(NewState);

	//DrawDebugSphere(GWorld, GetActorLocation(), 5, 10, FColor::Red, false, 5);

	FVector dirAway = (GetActorLocation() - SourceLocation);
	dirAway.Z = 0;
	dirAway.Normalize();
	int runawayDistance = FMath::RandRange(MaxDistance / 2, MaxDistance);
	FVector runawayTarget = GetActorLocation() + dirAway * runawayDistance;
	FVector inWaterTarget = Water->GetClosestPointInWater(runawayTarget);
	if (FVector::Distance(inWaterTarget, GetActorLocation()) < 200)
	{
		//UE_LOG(LogTemp, Log, TEXT("xxx runaway target too close"));
		FVector dirToRandom = Water->GetRandomPointInWater() - GetActorLocation();
		dirToRandom.Normalize();
		inWaterTarget = GetActorLocation() + dirToRandom * runawayDistance; 
	}
	
	//DrawDebugSphere(GWorld, inWaterTarget, 100, 10, FColor::Purple, false, 5);

	Water->UpdateInWaterTarget(inWaterTarget);
	blackboard->SetValueAsVector(FishBB_Target, inWaterTarget);
}

void AFishBase::OnRockPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnRockPerceptionUpdated = %s"), *Actor->GetName());

	// Actor is player character, not the rock!

	RunawayFrom(Stimulus.StimulusLocation, Data->RockRunawayDistance, EFishState::Rock);
}

void AFishBase::SetState(EFishState NewState)
{
	//UE_LOG(LogTemp, Log, TEXT("
	//xxx SetState %d"), (int)NewState);
	blackboard->SetValueAsEnum(FishBB_State, (int)NewState);
}

EFishState AFishBase::GetState()
{
	int state = blackboard->GetValueAsEnum(FishBB_State);
	return (EFishState)state;
}

