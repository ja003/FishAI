// Fill out your copyright notice in the Description page of Project Settings.


#include "Waterboarding.h"

#include "PlayerCharacter.h"
#include "FishAi/Waterboard.h"
#include "Kismet/KismetMathLibrary.h"


void UWaterboarding::OnPlayerBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult)
{
	if (AWaterboard* waterboard = Cast<AWaterboard>(Actor))
	{
		Waterboard = waterboard;
		float newYaw = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + Waterboard->GetActorForwardVector()).Yaw;
		GetOwner()->SetActorRotation(FRotator(0,newYaw,0));

		FVector newLocation = waterboard->BoardCollider->GetComponentLocation();
		newLocation.Z = GetOwner()->GetActorLocation().Z;
		GetOwner()->SetActorLocation(newLocation);
	}
}

void UWaterboarding::OnPlayerEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if (AWaterboard* waterboard = Cast<AWaterboard>(Actor))
	{
		Waterboard = nullptr;
	}
}
