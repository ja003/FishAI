// Fill out your copyright notice in the Description page of Project Settings.


#include "FishCarp.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"


void AFishCarp::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnBaitPerceptionUpdated"));

	blackboard->SetValueAsObject("Bait", Actor);
	blackboard->SetValueAsEnum("State", 1);

}

void AFishCarp::OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnPikePerceptionUpdated"));
}


