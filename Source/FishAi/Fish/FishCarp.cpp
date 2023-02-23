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
	//UE_LOG(LogTemp, Log, TEXT("xxx OnMouthBeginOverlap"));

	if(Cast<ABait>(Actor))
	{
		Cast<ABait>(Actor)->OnEaten();
		SetState(EFishState::Idle);
	}
}


void AFishCarp::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnBaitPerceptionUpdated %s"), *Actor->GetName());

	// if (GetState() != EFishState::Idle)
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("xxx ignoring bait"));
	// 	return;
	// }

	// gold fish ignores bait..to make it simpler
	if (FishType == EFish::Gold)
	{
		return;
	}

	SetState(EFishState::Bait);

	if (blackboard->GetValueAsObject(FishBB_Bait) != nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("xxx bait already set"));
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
		
		//blackboard->SetValueAsObject(FishBB_Bait, nullptr);		
		blackboard->SetValueAsVector(FishBB_Target, target);
		blackboard->SetValueAsBool("BaitVision", false);

	}

}

void AFishCarp::OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnPikePerceptionUpdated"));

	RunawayFrom(Actor->GetActorLocation(), Cast<UCarpData>(Data)->PikeRunawayDistance, EFishState::Danger);

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


