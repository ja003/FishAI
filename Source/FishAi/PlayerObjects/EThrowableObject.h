// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class UEThrowableObject : uint8 
{
	Rock     UMETA(DisplayName = "Rock"),
	Bait     UMETA(DisplayName = "Bait"),
	Grenade   UMETA(DisplayName = "Grenade")
};