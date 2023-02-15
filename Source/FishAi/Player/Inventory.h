// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishAi/PlayerObjects/ThrowableObject.h"
#include "FishAi/PlayerObjects/ThrowablesGenerator.h"
#include "Inventory.generated.h"


class AThrowableObject;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventory();

	TMap<EThrowableObject, int> Items;

	bool HasItem(EThrowableObject ObjectType);

	void RemoveItem(EThrowableObject Type);

	void AddItem(EThrowableObject ObjectType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetItemCount(EThrowableObject ObjectType);

	UPROPERTY(EditDefaultsOnly)
	TMap<EThrowableObject, TSubclassOf<AThrowableObject>> ThrowableObjectsBP;
};
