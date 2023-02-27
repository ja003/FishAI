// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableObject.h"
#include "FishAi/Constants.h"
#include "FishAi/StimuliObject.h"
#include "GameFramework/Actor.h"
#include "Bait.generated.h"

UCLASS()
class FISHAI_API ABait : public AThrowableObject
{
	GENERATED_BODY()

	ABait();

public:

	void OnEaten();

	virtual EStimuliObject GetStimuliType() override { return Bait; }

	virtual EThrowableObjectType GetType() override { return EThrowableObjectType::Bait; };

	virtual FName GetTag() override { return Tag_Bait; }

};
