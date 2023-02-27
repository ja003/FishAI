// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewardText.generated.h"

class UTextRenderComponent;

/**
 * Representation of reward received for killing a fish
 */
UCLASS()
class FISHAI_API ARewardText : public AActor
{
	GENERATED_BODY()

public:

	ARewardText();

	UPROPERTY(EditDefaultsOnly)
	UTextRenderComponent* Text;

	void SetReward(int Reward);

protected:

	virtual void BeginPlay() override;

};
