﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"


// Sets default values for this component's properties
UInventory::UInventory()
{
	Items.Add(EThrowableObject::Rock, 3);
	Items.Add(EThrowableObject::Bait, 2);
	Items.Add(EThrowableObject::Grenade, 1);
}

bool UInventory::HasItem(EThrowableObject ObjectType)
{
	return Items.Contains(ObjectType) && Items[ObjectType] > 0;
}

void UInventory::RemoveItem(EThrowableObject Type)
{
	Items[Type]--;
}

void UInventory::AddItem(EThrowableObject ObjectType)
{
	Items[ObjectType]++;
}

int UInventory::GetItemCount(EThrowableObject ObjectType)
{
	return Items[ObjectType];
}

