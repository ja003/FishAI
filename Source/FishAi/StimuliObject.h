// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EStimuliObject.h"
#include "UObject/Interface.h"
#include "StimuliObject.generated.h"

UINTERFACE()
class UStimuliSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * An object that creates some kind of stimuli source
 */
class FISHAI_API IStimuliSource
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual	EStimuliObject GetStimuliType() = 0;
};
