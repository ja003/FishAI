// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish/FishCarp.h"

void AFishCarp::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	Super::OnTargetPerceptionUpdated(Actor, Stimulus);

	UE_LOG(LogTemp, Log, TEXT("xxx AFishCarp::OnTargetPerceptionUpdated = %s"), *Actor->GetName());
}
