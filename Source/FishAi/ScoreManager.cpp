// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreManager.h"


void AScoreManager::OnFishKilled(EFish Fish)
{
	switch (Fish)
	{
	case EFish::Carp: 
		Score += 3;
		break;
	case EFish::Pike: 
		Score += 1;
		break;
	default: ;
	}
}
