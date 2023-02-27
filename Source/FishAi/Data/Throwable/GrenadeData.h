﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableData.h"
#include "GrenadeData.generated.h"

UCLASS()
class FISHAI_API UGrenadeData : public UThrowableData
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditDefaultsOnly)
	int Radius;
	
};
