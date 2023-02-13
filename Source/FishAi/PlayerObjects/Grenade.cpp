// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

void AGrenade::OnEnteredWater()
{
	Super::OnEnteredWater();

	UE_LOG(LogTemp, Log, TEXT("xxx explode"));
}
