// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"

#include "WaterBodyActor.h"
#include "Components/SphereComponent.h"
#include "FishAi/Constants.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/Character.h"


ARock::ARock()
{
	//SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ARock::OnComponentBeginOverlap);

	SphereCollider->OnComponentHit.AddDynamic(this, &ARock::OnComponentHit);

	// todo: this doesnt work
	// StimuliSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
	// StimuliSource->bAutoRegister = true;
}

void ARock::OnEnteredWater()
{
	Super::OnEnteredWater();

	// todo: this doesnt work
	ACharacter* noiseInstigator = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	MakeNoise(1, noiseInstigator, GetActorLocation());
	//UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation());

	ReportNoise();
}

void ARock::OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult)
{
	if (bHasHitGround) return;

	if(Actor->ActorHasTag(Tag_Ground))
	{
		UE_LOG(LogTemp, Log, TEXT("xxx Ground!"));
		bHasHitGround = true;

		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ARock::DestroyRock, 1, false);
	}
}

void ARock::DestroyRock()
{
	Destroy();
}

