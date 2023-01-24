// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EStimuliObject.h"
#include "UObject/Interface.h"
#include "StimuliObject.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UStimuliSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHAI_API IStimuliSource
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual	EStimuliObject GetStimuliType() = 0;
};
