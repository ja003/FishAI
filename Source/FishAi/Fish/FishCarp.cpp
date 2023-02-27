// Fill out your copyright notice in the Description page of Project Settings.


#include "FishCarp.h"

#include "EFishState.h"
#include "Components/CapsuleComponent.h"
#include "FishAi/Constants.h"
#include "FishAi/Managers/WaterManager.h"
#include "FishAi/Data/Fish/CarpData.h"
#include "FishAi/Data/DataManager.h"
#include "FishAi/Throwable/Bait.h"


void AFishCarp::OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	if(Cast<ABait>(Actor))
	{
		Cast<ABait>(Actor)->OnEaten();
		SetState(EFishState::Idle);
	}
}


void AFishCarp::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	// gold fish ignores bait..to make it easier for a player
	if (FishType == EFish::Gold)
	{
		return;
	}

	SetState(EFishState::Bait);

	if (blackboard->GetValueAsObject(FishBB_Bait) != nullptr)
	{
		// bait already set
		return;
	}
	
	// if perceived by sound => Actor is not the Bait => go search to sound location 
	if(Actor->ActorHasTag(Tag_Bait))
	{
		blackboard->SetValueAsObject(FishBB_Bait, Actor);
		blackboard->SetValueAsBool("BaitVision", true);
	}
	else
	{
		FVector target = Stimulus.StimulusLocation;
		if (!Water->IsPointInWater(target))
		{
			SetState(EFishState::Idle);
			return;
		}
		
		Water->UpdateInWaterTarget(target);
		
		blackboard->SetValueAsVector(FishBB_Target, target);
		blackboard->SetValueAsBool("BaitVision", false);
	}
}

void AFishCarp::OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	RunawayFrom(Actor->GetActorLocation(), Cast<UCarpData>(Data)->PikeRunawayDistance, EFishState::Danger);
}


