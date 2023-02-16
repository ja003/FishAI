// Fill out your copyright notice in the Description page of Project Settings.


#include "NavModifier.h"

//#include "NavModifierComponent.h"


// Sets default values
ANavModifier::ANavModifier()
{
	//NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT(""));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("box"));
	RootComponent = Collider;

	Collider->bDynamicObstacle = true;
	Collider->RegisterComponent();
}
