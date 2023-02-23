// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "WaterManager.h"
#include "FishAi/Player/PlayerCharacter.h"

void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (WaterManagers.Num() == 0)
		return;

	for (auto levelTrigger : NextLevelTriggers)
	{
		levelTrigger->SetActorHiddenInGame(true);	
	}

	TriggerNextLevel();	
}

void AGameManager::OnAllFishesDead()
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnAllFishesDead"));
	
	if (NextLevelTriggers.Num() <= currentLevel)
	{
		// end game
		UE_LOG(LogTemp, Log, TEXT("xxx end game"));
		return;
	}

	NextLevelTriggers[currentLevel]->SetActorHiddenInGame(false);
	NextLevelTriggers[currentLevel]->TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AGameManager::OnNextLevelTriggerOverlap);
}

void AGameManager::OnNextLevelTriggerOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnNextLevelTriggerOverlap"));
	
	if (Cast<APlayerCharacter>(Actor))
		TriggerNextLevel();
}

void AGameManager::TriggerNextLevel()
{
	currentLevel++;
	UE_LOG(LogTemp, Log, TEXT("xxx TriggerNextLevel %d"), currentLevel);
	WaterManagers[currentLevel]->Init();
	WaterManagers[currentLevel]->OnAllFishesDead.AddDynamic(this, &AGameManager::OnAllFishesDead);
}
