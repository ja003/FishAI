// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "FishAi/Data/PlayerData.h"

void UInventory::BeginPlay()
{
	Super::BeginPlay();

	for (auto item : Data->StartingItems)
	{
		Items.Add(item.Key, item.Value);
	}
}

bool UInventory::HasItem(EThrowableObjectType ObjectType)
{
	return Items.Contains(ObjectType) && Items[ObjectType] > 0;
}

void UInventory::RemoveItem(EThrowableObjectType Type)
{
	if(!bDebug_InfiniteItems)
		Items[Type]--;
}

void UInventory::AddItem(EThrowableObjectType ObjectType)
{
	Items[ObjectType]++;
}

int UInventory::GetItemCount(EThrowableObjectType ObjectType)
{
	return Items[ObjectType];
}

