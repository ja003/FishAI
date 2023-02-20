// Fill out your copyright notice in the Description page of Project Settings.


#include "Bait.h"


ABait::ABait()
{
	Tags.Add(Tag_Bait);
}

void ABait::OnEaten()
{
	//UE_LOG(LogTemp, Log, TEXT("xxx ABait::OnEaten"));
	Destroy();
}
