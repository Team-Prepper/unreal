// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include <algorithm>

Inventory::Inventory()
{
	
}

Inventory::~Inventory()
{

}

bool Inventory::InventoryUnit::compare(InventoryUnit &a, InventoryUnit &b) {
	if (a.itemAmount == 0) return false;
	if (b.itemAmount == 0) return true;

	return a.itemCode < b.itemCode;
}

void Inventory::SortInventory() {
	std::sort(_units, _units + INVENTORY_CAPACITY, InventoryUnit::compare);
}

bool Inventory::TryAddItem(int32 itemCode) {
	// 아이템을 추가할 위치
	int targetPos = -1;

	for (int i = 0; i < INVENTORY_CAPACITY; i++) {
		// 동일한 아이템이 있고, 용량을 초과하지 않는 조건을 만족하는 첫 번째 위치에 추가
		if (i == itemCode && _units[i].itemAmount < INVENTORY_UNIT_CAPACITY) {
			targetPos = i;
			break;
		}
		if (targetPos != -1) continue;
		// 모든 슬롯을 확인했을 때 동일한 아이템이 없거나 모두 용량이 가득찼다면 첫 번째 빈칸에 추가
		if (_units[i].itemAmount == 0) targetPos = i;
	}

	// targetPos가 -1이면 아이템을 추가할 공간을 발견하지 못한 것
	if (targetPos == -1)
		return false;

	_units[targetPos].itemCode = itemCode;
	_units[targetPos].itemAmount++;

	return true;

}

bool Inventory::TryAddItemAt(int32 itemCode, uint32 idx) {

	// idx에 다른 아이템이 이미 있다면 추가 불가능
	if (_units[idx].itemAmount != itemCode && _units[idx].itemAmount > 0) {
		return false;
	}

	_units[idx].itemCode = itemCode;
	_units[idx].itemAmount++;

	return true;

}

uint32 Inventory::GetItemAmountAt(uint32 idx) {
	return _units[idx].itemAmount;
}

int32 Inventory::GetItemCodeAt(uint32 idx) {
	if (GetItemAmountAt(idx) == 0)
		return -1;

	return _units[idx].itemCode;
	
}