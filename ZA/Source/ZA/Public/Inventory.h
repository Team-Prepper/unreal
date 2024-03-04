// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define INVENTORY_CAPACITY 200
#define INVENTORY_UNIT_CAPACITY 99

/**
 * 
 */
class ZA_API Inventory
{
private:
	struct InventoryUnit {
		int32 itemCode = -1;
		uint32 itemAmount = 0;

		static bool compare(InventoryUnit &a, InventoryUnit &b);
	};

	InventoryUnit _units[INVENTORY_CAPACITY];

public:
	Inventory();
	~Inventory();

	void SortInventory();
	
	bool TryAddItem(int32 itemCode);
	bool TryAddItemAt(int32 itemCode, uint32 idx);

	uint32 GetItemAmountAt(uint32 idx);
	int32 GetItemCodeAt(uint32 idx);
};
