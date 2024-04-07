// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataGetter.h"

#define MAX_ITEM_COUNT 16

class PREPPER_API Inventory
{
private:
	ItemDataGetter ItemData;
	uint8 BulletCount;
	
	TMap<FString, uint8> ItemUnits;

	bool TryCombineItem(const FString& Input1, const FString& Input2, FString& Result);
public:
	Inventory();
	~Inventory();
	bool TryAddItem(const FString& ItemCode);
	bool TryUseItem(const FString& ItemCode);
	
	void AddBullet(uint8 Count);
	uint8 GetBulletCount() const;
	
};
