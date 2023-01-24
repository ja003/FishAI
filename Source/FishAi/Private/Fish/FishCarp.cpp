// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish/FishCarp.h"


void AFishCarp::OnBaitPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnBaitPerceptionUpdated"));

}

void AFishCarp::OnPikePerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnPikePerceptionUpdated"));
}


