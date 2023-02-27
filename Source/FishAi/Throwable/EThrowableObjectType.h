// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EThrowableObjectType : uint8 
{
	None     UMETA(DisplayName = "None"),
	Rock     UMETA(DisplayName = "Rock"),
	Bait     UMETA(DisplayName = "Bait"),
	Grenade   UMETA(DisplayName = "Grenade")
};