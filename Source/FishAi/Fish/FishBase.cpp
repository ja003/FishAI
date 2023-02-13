// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

#include "EFishState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Perception/AIPerceptionComponent.h"

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

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AFishBase::OnTargetPerceptionUpdated);

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

}

// Called when the game starts or when spawned
void AFishBase::BeginPlay()
{
	Super::BeginPlay();

	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

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
}

void AFishBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	bodyMesh->SetRelativeLocation(FVector(0,0, -GetActorLocation().Z));
}

void AFishBase::OnKilledByPlayer()
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnKilledByPlayer"));

	Die();
}

void AFishBase::Die()
{
	Destroy();
}

void AFishBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnTargetPerceptionUpdated = %s"), *Actor->GetName());

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
}

void AFishBase::OnRockPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnRockPerceptionUpdated = %s"), *Actor->GetName());

	// Actor is player character, not the rock!

	blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Rock);

	DrawDebugSphere(GWorld, GetActorLocation(), 5, 10, FColor::Red, false, 5);

	FVector dirAway = (GetActorLocation() - Stimulus.StimulusLocation);
	dirAway.Normalize();
	FVector runawayTarget = GetActorLocation() + dirAway * 200;
	DrawDebugSphere(GWorld, runawayTarget, 50, 10, FColor::Blue, true, 5);
	
	blackboard->SetValueAsVector(FishBB_Target, runawayTarget);

}

