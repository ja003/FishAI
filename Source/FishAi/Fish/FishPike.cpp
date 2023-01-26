#include "FishPike.h"

#include "EFishState.h"
#include "FishAi/Constants.h"

void AFishPike::OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	Super::OnEdibleFishPerceptionUpdated(Actor, Stimulus);

	//UE_LOG(LogTemp, Log, TEXT("xxx IsReadyForHunt = %s"), IsReadyForHunt() ? TEXT("true"):TEXT("false"));
	
	if(IsReadyForHunt())
	{
		blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Hunt);
		blackboard->SetValueAsObject(FishBB_Prey, Actor);

		lastHuntTime = FDateTime::Now().ToUnixTimestamp();
	}
}

void AFishPike::OnComponentHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Normal, const FHitResult& HitResult)
{
	Super::OnComponentHit(PrimitiveComponent, Actor, PrimitiveComponent1, Normal, HitResult);

	UE_LOG(LogTemp, Log, TEXT("xxx OnComponentHit"));

	if(Cast<AFishBase>(Actor))
	{
		Cast<AFishBase>(Actor)->Die();
	}
}


bool AFishPike::IsReadyForHunt()
{
	return FDateTime::Now().ToUnixTimestamp() - lastHuntTime > minHuntCooldown;
}
