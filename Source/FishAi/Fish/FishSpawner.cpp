// Fill out your copyright notice in the Description page of Project Settings.


#include "FishSpawner.h"


AFishBase* AFishSpawner::SpawnFish(EFish FishType, FVector Location)
{
	FTransform spawnTransform;
	spawnTransform.SetLocation(Location);
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	switch (FishType)
	{
	case EFish::Carp:
		{
			return GetWorld()->SpawnActor<AFishBase>(CarpBP, spawnTransform, spawnParameters);
		}
	case EFish::Pike:
		return GetWorld()->SpawnActor<AFishBase>(PikeBP, spawnTransform);		
	}


	checkNoEntry()
	return nullptr;
}
