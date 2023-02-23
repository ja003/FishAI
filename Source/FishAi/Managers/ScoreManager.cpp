// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreManager.h"

#include "FishAi/Data/DataManager.h"
#include "FishAi/Fish/FishBase.h"
#include "Kismet/GameplayStatics.h"


void AScoreManager::BeginPlay()
{
	Super::BeginPlay();

	Data = Cast<ADataManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADataManager::StaticClass()));
	check(Data)
}

void AScoreManager::OnFishKilled(AFishBase* Fish)
{
	FTransform spawnTransform = Fish->GetActorTransform();
	spawnTransform.SetLocation(FVector(
		spawnTransform.GetLocation().X,
		spawnTransform.GetLocation().Y,
		100));
	
	ARewardText* text = GetWorld()->SpawnActor<ARewardText>(RewardTextBP, spawnTransform);
	
	int reward = Data->Fish[Fish->FishType]->Reward;
	
	text->SetReward(reward);
	Score += reward;
	
}
