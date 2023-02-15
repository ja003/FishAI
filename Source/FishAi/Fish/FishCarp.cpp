// Fill out your copyright notice in the Description page of Project Settings.


#include "FishCarp.h"

#include "EFishState.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/Data/DataManager.h"
#include "FishAi/PlayerObjects/Bait.h"


void AFishCarp::OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnMouthBeginOverlap"));

	if(Cast<ABait>(Actor))
	{
		Cast<ABait>(Actor)->OnEaten();
	}
}


void AFishCarp::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnBaitPerceptionUpdated"));

	blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Bait);
	blackboard->SetValueAsObject(FishBB_Bait, Actor);

}

void AFishCarp::OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnPikePerceptionUpdated"));

	RunawayFrom(Actor->GetActorLocation(), Data->Fish->PikeRunawayDistance, EFishState::Danger);

	// blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Danger);
	//
	//
	// FVector dirAway = (GetActorLocation() - Actor->GetActorLocation());
	// dirAway.Normalize();
	// FVector runawayTarget = GetActorLocation() + dirAway * 400;
	// DrawDebugSphere(GWorld, runawayTarget, 5, 10, FColor::Blue, false, 5);
	//
	// blackboard->SetValueAsVector(FishBB_Target, runawayTarget);
}


