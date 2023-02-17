// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishAi/Waterboard.h"
#include "Waterboarding.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UWaterboarding : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool Arg, const FHitResult& HitResult);
	
	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	UPROPERTY(BlueprintReadOnly)
	AWaterboard* Waterboard;

};
