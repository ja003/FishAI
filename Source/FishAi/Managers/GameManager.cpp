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
	if (NextLevelTriggers.Num() <= currentLevel)
	{
		// end game
		EndGameBP();
		return;
	}

	NextLevelTriggers[currentLevel]->SetActorHiddenInGame(false);
	NextLevelTriggers[currentLevel]->TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AGameManager::OnNextLevelTriggerOverlap);

	OnAllFishesDeadBP();
}

void AGameManager::OnNextLevelTriggerOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	if (Cast<APlayerCharacter>(Actor))
	{
		NextLevelTriggers[currentLevel]->SetActorHiddenInGame(true);
		TriggerNextLevel();
	}
}

void AGameManager::TriggerNextLevel()
{
	currentLevel++;
	WaterManagers[currentLevel]->Init();
	WaterManagers[currentLevel]->OnAllFishesDead.AddDynamic(this, &AGameManager::OnAllFishesDead);
}
