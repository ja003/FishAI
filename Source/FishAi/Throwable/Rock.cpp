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
	// todo: this doesnt work
	// StimuliSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
	// StimuliSource->bAutoRegister = true;

	Tags.Add(Tag_Rock);
}
