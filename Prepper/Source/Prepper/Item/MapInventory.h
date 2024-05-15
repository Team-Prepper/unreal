// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData/ItemDataGetter.h"
#include "Prepper/Interfaces/Inventory.h"
//#include "MapInventory.generated.h"

/**
 * 
 */
//UCLASS()
class PREPPER_API UMapInventory : public IInventory
{
	//GENERATED_BODY()
private:
	ItemDataGetter ItemData;
	uint8 BulletCount;
	
	TMap<FString, uint8> ItemUnits;

	bool TryCombineItem(const FString& Input1, const FString& Input2, FString& Result);
public:
	UMapInventory();
	virtual bool TryAddItem(const FString& ItemCode) override;
	virtual bool TryUseItem(const FString& ItemCode) override;
	virtual TArray<InventoryItem> GetIter() override;

	
	void AddBullet(uint8 Count);
	uint8 GetBulletCount() const;

};
