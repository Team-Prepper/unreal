// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define INVENTORY_CAPACITY 200
#define INVENTORY_UNIT_CAPACITY 99

/**
 * 
 */
class ZA_API FInventory
{

public:
	FInventory();
	~FInventory();

	void SortInventory();
	
	bool TryAddItem(int32 ItemCode);
	bool TryAddItemAt(int32 ItemCode, uint32 Idx);

	uint32 GetItemAmountAt(uint32 Idx) const;
	int32 GetItemCodeAt(uint32 Idx) const;
private:
	struct FInventoryUnit {
		int32 ItemCode = -1;
		uint32 ItemAmount = 0;

		static bool Compare(const FInventoryUnit &A, const FInventoryUnit &B);
	};

	FInventoryUnit Units[INVENTORY_CAPACITY];

};
