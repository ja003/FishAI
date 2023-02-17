// Fill out your copyright notice in the Description page of Project Settings.


#include "Waterboarding.h"

#include "PlayerCharacter.h"
#include "FishAi/Waterboard.h"
#include "Kismet/KismetMathLibrary.h"


void UWaterboarding::OnPlayerBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnPlayerBeginOverlap"));

	if (AWaterboard* waterboard = Cast<AWaterboard>(Actor))
	{
		Waterboard = waterboard;
		//UE_LOG(LogTemp, Log, TEXT("xxx AWaterboard"));

		float newYaw = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + Waterboard->GetActorForwardVector()).Yaw;
		GetOwner()->SetActorRotation(FRotator(0,newYaw,0));
	}
}

void UWaterboarding::OnPlayerEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	//UE_LOG(LogTemp, Log, TEXT("xxx OnPlayerEndOverlap"));

	if (AWaterboard* waterboard = Cast<AWaterboard>(Actor))
	{
		Waterboard = nullptr;
		//UE_LOG(LogTemp, Log, TEXT("xxx end waterboarding"));
	}
}
