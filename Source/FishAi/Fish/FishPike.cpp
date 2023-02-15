#include "FishPike.h"

#include "EFishState.h"
#include "FishAi/Constants.h"
#include "FishAi/WaterManager.h"
#include "FishAi/Data/DataManager.h"
#include "FishAi/Managers/NoiseReporter.h"
#include "Kismet/GameplayStatics.h"

AFishPike::AFishPike()
{
}

void AFishPike::OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("xxx OnEdibleFishPerceptionUpdated"));

	Super::OnEdibleFishPerceptionUpdated(Actor, Stimulus);

	UE_LOG(LogTemp, Log, TEXT("xxx IsReadyForHunt = %s"), IsReadyForHunt() ? TEXT("true"):TEXT("false"));
	
	if(IsReadyForHunt())
	{
		UE_LOG(LogTemp, Log, TEXT("xxx go hunt"));
		
		blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Hunt);
		blackboard->SetValueAsObject(FishBB_Prey, Actor);

		lastHuntTime = FDateTime::Now().ToUnixTimestamp();

		GetWorld()->GetTimerManager().ClearTimer(EndHuntHandle);
		GetWorld()->GetTimerManager().SetTimer(EndHuntHandle, this, &AFishPike::EndHunt, Data->Fish->HuntDuration, false);
	}
}

void AFishPike::BeginPlay()
{
	Super::BeginPlay();

	currentPatrolPathIndex = FMath::RandRange(0, 5);

	GetWorld()->GetTimerManager().SetTimer(RoarHandle, this, &AFishPike::Roar, Data->Fish->RoarCoolDown, true);

}

void AFishPike::OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult)
{
	Super::OnComponentHit(PrimitiveComponent, Actor, PrimitiveComponent1, Normal, HitResult);

	//UE_LOG(LogTemp, Log, TEXT("xxx OnComponentHit"));

	if(Cast<AFishBase>(Actor))
	{
		Cast<AFishBase>(Actor)->Die();
	}
}


bool AFishPike::IsReadyForHunt()
{
	return FDateTime::Now().ToUnixTimestamp() - lastHuntTime > Data->Fish->HuntCooldown;
}

void AFishPike::OnMouthBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	Super::OnMouthBeginOverlap(PrimitiveComponent, Actor, PrimitiveComponent1, I, Arg, HitResult);

	UE_LOG(LogTemp, Log, TEXT("xxx AFishPike::OnMouthBeginOverlap"));

	if(AFishBase* fish = Cast<AFishBase>(Actor))
	{
		if (fish->Type != EFish::Pike)
			fish->Die();
	}
}

void AFishPike::EndHunt()
{
	UE_LOG(LogTemp, Log, TEXT("xxx end hunt"));
	blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Idle);
}

void AFishPike::Roar()
{
	NoiseReporter->ReportNoise(this, GetActorLocation(), nullptr, 1, Data->Fish->RoarRange);
	//UE_LOG(LogTemp, Log, TEXT("xxx roar"));
}

FVector AFishPike::GetNextPatrolPoint()
{
	if (Water == nullptr || Water->PatrolPath.Num() == 0)
	{
		return GetActorLocation();
	}
	
	currentPatrolPathIndex++;
	return  Water->PatrolPath[(currentPatrolPathIndex - 1) % Water->PatrolPath.Num()];
}
