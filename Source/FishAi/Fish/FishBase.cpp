// Fill out your copyright notice in the Description page of Project Settings.


#include "FishBase.h"

#include "EFishState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FishAi/Constants.h"
#include "FishAi/StimuliObject.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AFishBase::AFishBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("body"));
	bodyMesh->AddLocalRotation(FRotator(0,-90,0));
	bodyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	bodyMesh->SetCanEverAffectNavigation(false);

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AFishBase::OnTargetPerceptionUpdated);

}

// Called when the game starts or when spawned
void AFishBase::BeginPlay()
{
	Super::BeginPlay();

	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
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
	DrawDebugSphere(GWorld, runawayTarget, 5, 10, FColor::Blue, false, 5);
	
	blackboard->SetValueAsVector(FishBB_Target, runawayTarget);

}

