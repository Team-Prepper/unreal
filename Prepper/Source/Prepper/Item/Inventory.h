// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define MAX_ITEM_COUNT 16

class PREPPER_API Inventory
{
private:
	uint8 BulletCount;
	
	TMap<FString, uint8> ItemUnits;
public:
	Inventory();
	~Inventory();
	bool TryAddItem(const FString& ItemCode);
	bool TryUseItem(const FString& ItemCode);
	void AddBullet(uint8 Count);
	uint8 GetBulletCount() const;
};
