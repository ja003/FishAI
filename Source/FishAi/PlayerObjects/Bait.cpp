// Fill out your copyright notice in the Description page of Project Settings.


#include "Bait.h"


// Sets default values
ABait::ABait()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABait::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABait::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

