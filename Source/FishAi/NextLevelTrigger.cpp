// Fill out your copyright notice in the Description page of Project Settings.


#include "NextLevelTrigger.h"

#include "Components/BoxComponent.h"


// Sets default values
ANextLevelTrigger::ANextLevelTrigger()
{
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	TriggerArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
