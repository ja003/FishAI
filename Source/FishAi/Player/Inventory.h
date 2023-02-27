// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishAi/Throwable/ThrowableObject.h"
#include "FishAi/Throwable/ThrowablesGenerator.h"
#include "Inventory.generated.h"


class UPlayerData;
class AThrowableObject;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FISHAI_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	TMap<EThrowableObjectType, int> Items;

	bool HasItem(EThrowableObjectType ObjectType);

	void RemoveItem(EThrowableObjectType Type);

	void AddItem(EThrowableObjectType ObjectType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetItemCount(EThrowableObjectType ObjectType);

	UPROPERTY(EditDefaultsOnly)
	TMap<EThrowableObjectType, TSubclassOf<AThrowableObject>> ThrowableObjectsBP;

	UPROPERTY(EditAnywhere)
	bool bDebug_InfiniteItems;

	UPROPERTY(EditDefaultsOnly)
	UPlayerData* Data;
};
