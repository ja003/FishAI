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
	ARewardText* text = GetWorld()->SpawnActor<ARewardText>(RewardTextBP, Fish->GetActorTransform());
	
	int reward = Data->Fish[Fish->Type]->Reward;
	
	text->SetReward(reward);
	Score += reward;
	
}
