// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
//#include <algorithm>

Inventory::Inventory()
{
	
}

Inventory::~Inventory()
{

}

bool Inventory::InventoryUnitCompare(InventoryUnit a, InventoryUnit b) {
	if (a.itemAmount == 0) return false;
	if (b.itemAmount == 0) return true;

	return a.itemCode < b.itemCode;
}

void Inventory::SortAll() {
	//std::sort(_units, _units + INVENTORY_CAPACITY, InventoryUnitCompare);
}

void Inventory::AddItem(int itemCode) {
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
		return;

	_units[targetPos].itemCode = itemCode;
	_units[targetPos].itemAmount++;

}

void Inventory::AddItemAt(int itemCode, int idx) {

	// idx에 다른 아이템이 이미 있다면 추가 불가능
	if (_units[idx].itemAmount != itemCode && _units[idx].itemAmount > 0) {
		return;
	}

	_units[idx].itemCode = itemCode;
	_units[idx].itemAmount++;

}

int Inventory::GetItemAmountAt(int idx) {
	return _units[idx].itemAmount;
}

int Inventory::GetItemCodeAt(int idx) {
	return _units[idx].itemCode;
	
}