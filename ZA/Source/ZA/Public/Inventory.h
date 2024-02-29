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
		int itemCode = 0;
		int itemAmount = 0;

	};
	bool InventoryUnitCompare(InventoryUnit a, InventoryUnit b);

	InventoryUnit _units[INVENTORY_CAPACITY];

public:
	Inventory();
	~Inventory();

	void SortAll();
	void AddItem(int itemCode);
	void AddItemAt(int itemCode, int idx);
	int GetItemAmountAt(int idx);
	int GetItemCodeAt(int idx);
};
