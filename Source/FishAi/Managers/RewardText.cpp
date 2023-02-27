// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardText.h"

#include "Components/TextRenderComponent.h"


ARewardText::ARewardText()
{
	PrimaryActorTick.bCanEverTick = true;

	Text = CreateDefaultSubobject<UTextRenderComponent>("text");
	RootComponent = Text;
}

void ARewardText::SetReward(int Reward)
{
	const char* signStr = (Reward > 0 ? "+" : "-");
	FText text = FText::Format(FText::FromString("{0}{1}"), FText::FromString(signStr), Reward);
	Text->SetText(text);

	Text->TextRenderColor = Reward > 0 ? FColor::Green : FColor::Red;
}

void ARewardText::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3);	
}

