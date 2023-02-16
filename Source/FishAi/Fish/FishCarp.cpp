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
		blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Idle);
	}
}


void AFishCarp::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnBaitPerceptionUpdated %s"), *Actor->GetName());
	blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Bait);

	// if perceived by sound => Actor is not the Bait => go search to sound location 
	if(Actor->ActorHasTag(Tag_Bait))
	{
		blackboard->SetValueAsObject(FishBB_Bait, Actor);
		blackboard->SetValueAsBool("BaitVision", true);
		
	}
	else
	{
		//blackboard->SetValueAsObject(FishBB_Bait, nullptr);		
		blackboard->SetValueAsVector(FishBB_Target, Stimulus.StimulusLocation);
		blackboard->SetValueAsBool("BaitVision", false);

	}

}

void AFishCarp::OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnPikePerceptionUpdated"));

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


