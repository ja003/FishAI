// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

#include "EFishState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/StimuliObject.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AFishBase::AFishBase()
{
	// BP equivalent of "simulation generates hit events"
	// ..not rly sure if necessary, hit event is triggered even without it
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	
	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("body"));
	bodyMesh->AddLocalRotation(FRotator(0,-90,0));
	bodyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	bodyMesh->SetCanEverAffectNavigation(false);
	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AFishBase::OnTargetPerceptionUpdated);

}

// Called when the game starts or when spawned
void AFishBase::BeginPlay()
{
	Super::BeginPlay();

	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFishBase::OnComponentBeginOverlap);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFishBase::OnComponentHit);

	//Target = GetWorld()->SpawnActor<ATargetObject>();
}

void AFishBase::Die()
{
	Destroy();
}

void AFishBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
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
	DrawDebugSphere(GWorld, runawayTarget, 5, 10, FColor::Blue, false, 5);
	
	blackboard->SetValueAsVector(FishBB_Target, runawayTarget);

}

