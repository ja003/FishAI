// Fill out your copyright notice in the Description page of Project Settings.


#include "FishSpawner.h"

#include "FishAi/Fish/FishBase.h"


AFishBase* AFishSpawner::SpawnFish(EFish FishType, FVector Location)
{
	FTransform spawnTransform;
	spawnTransform.SetLocation(Location);
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	check(FishBPs.Contains(FishType))
	
	return GetWorld()->SpawnActor<AFishBase>(FishBPs[FishType], spawnTransform, spawnParameters);
}
