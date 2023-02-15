// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreManager.h"


void AScoreManager::OnFishKilled(AFishBase* Fish)
{
	ARewardText* text = GetWorld()->SpawnActor<ARewardText>(RewardTextBP, Fish->GetActorTransform());
	
	int reward = 0;
	switch (Fish->GetType())
	{
	case EFish::Carp: 
		reward = 3;
		break;
	case EFish::Pike: 
		reward = 1;
		break;
	default: ;
	}
	
	text->SetReward(reward);
	Score += reward;
	
}
