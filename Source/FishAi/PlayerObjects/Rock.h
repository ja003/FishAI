// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableObject.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"

UCLASS()
class FISHAI_API ARock : public AThrowableObject
{
	GENERATED_BODY()

public:

	virtual EStimuliObject GetStimuliType() override { return Rock; }

};
