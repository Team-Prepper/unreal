// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory.generated.h"
#define MAX_ITEM_COUNT 16

class IPlayerAbility;
UINTERFACE(MinimalAPI)
class UInventory : public UInterface
{
	GENERATED_BODY()
};

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
	
	virtual bool TryAddItem(const FString& ItemCode) PURE_VIRTUAL(IInventory::TryAddItem, return 0; ); 
	virtual bool TryUseItem(const FString& ItemCode) PURE_VIRTUAL(IInventory::TryUseItem, return 0; );
	virtual bool TryDiscardItem(const FString& ItemCode) PURE_VIRTUAL(IInventory::TryDiscardItem, return 0; );

	virtual int TryGetItemCount(const FString& ItemCode) PURE_VIRTUAL(IInventory::TryGetItemCount, return 0; );
	
	virtual bool CheckOwnItem(const FString& ItemCode) PURE_VIRTUAL(IInventory::CheckOwnItem, return 0; );
	virtual void QuickSlotAdd(const FString& ItemCode, const int Idx) PURE_VIRTUAL(); 
	virtual void UseItemAtQuickSlot(const int Idx) PURE_VIRTUAL();

	virtual TArray<InventoryItem> GetIter() PURE_VIRTUAL(IInventory::GetIter, TArray<InventoryItem> Retval; return Retval; );
};
