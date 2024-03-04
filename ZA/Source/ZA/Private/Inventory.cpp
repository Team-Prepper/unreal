// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include <algorithm>

FInventory::FInventory()
{
	
}

FInventory::~FInventory()
{

}

bool FInventory::FInventoryUnit::Compare(const FInventoryUnit &A, const FInventoryUnit &B) {
	if (A.ItemAmount == 0) return false;
	if (B.ItemAmount == 0) return true;

	return A.ItemCode < B.ItemCode;
}

void FInventory::SortInventory() {
	std::sort(Units, Units + INVENTORY_CAPACITY, FInventoryUnit::Compare);
}

bool FInventory::TryAddItem(const int32 ItemCode) {
	// 아이템을 추가할 위치
	int TargetPos = -1;

	for (int i = 0; i < INVENTORY_CAPACITY; i++) {
		// 동일한 아이템이 있고, 용량을 초과하지 않는 조건을 만족하는 첫 번째 위치에 추가
		if (i == ItemCode && Units[i].ItemAmount < INVENTORY_UNIT_CAPACITY) {
			TargetPos = i;
			break;
		}
		if (TargetPos != -1) continue;
		// 모든 슬롯을 확인했을 때 동일한 아이템이 없거나 모두 용량이 가득찼다면 첫 번째 빈칸에 추가
		if (Units[i].ItemAmount == 0) TargetPos = i;
	}

	// targetPos가 -1이면 아이템을 추가할 공간을 발견하지 못한 것
	if (TargetPos == -1)
		return false;

	Units[TargetPos].ItemCode = ItemCode;
	Units[TargetPos].ItemAmount++;

	return true;

}

bool FInventory::TryAddItemAt(const int32 ItemCode, uint32 Idx) {

	// idx에 다른 아이템이 이미 있다면 추가 불가능
	if (Units[Idx].ItemAmount != ItemCode && Units[Idx].ItemAmount > 0) {
		return false;
	}

	Units[Idx].ItemCode = ItemCode;
	Units[Idx].ItemAmount++;

	return true;

}

uint32 FInventory::GetItemAmountAt(const uint32 Idx) const
{
	return Units[Idx].ItemAmount;
}

int32 FInventory::GetItemCodeAt(const uint32 Idx) const
{
	if (GetItemAmountAt(Idx) == 0)
		return -1;

	return Units[Idx].ItemCode;
	
}
