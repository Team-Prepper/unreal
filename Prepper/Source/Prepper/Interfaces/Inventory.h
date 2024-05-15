// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory.generated.h"
#define MAX_ITEM_COUNT 16

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventory : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IInventory
{
	GENERATED_BODY()

public:
	class InventoryItem
	{
	public:
		InventoryItem(const FString& InitItemCode, const uint8 InitCount) { ItemCode = InitItemCode; Count = InitCount;};
		FString ItemCode;
		uint8 Count;
	};
	
	virtual bool TryAddItem(const FString& ItemCode) PURE_VIRTUAL(IIInventory::TryAddItem, return 0; ); 
	virtual bool TryUseItem(const FString& ItemCode) PURE_VIRTUAL(IIInventory::TryUseItem, return 0; );

	virtual TArray<InventoryItem> GetIter() PURE_VIRTUAL(IIInventory::GetIter, TArray<InventoryItem> Retval; return Retval; );
};
