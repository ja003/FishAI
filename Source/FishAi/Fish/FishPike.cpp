#include "FishPike.h"

#include "EFishState.h"
#include "FishAi/Constants.h"

void AFishPike::OnEdibleFishPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus)
{
	Super::OnEdibleFishPerceptionUpdated(Actor, Stimulus);

	blackboard->SetValueAsEnum(FishBB_State, (int)EFishState::Hunt);
	blackboard->SetValueAsObject(FishBB_Prey, Actor);
}
