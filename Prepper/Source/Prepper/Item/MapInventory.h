// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MAX_QUICK_SLOT 5

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/ItemManager.h"
#include "Prepper/Interfaces/Inventory.h"
#include "MapInventory.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UMapInventory : public UActorComponent, public IInventory
{
	GENERATED_BODY()
private:
	IPlayerAbility * Owner;
	
	uint8 BulletCount;
	
	TMap<FString, uint8> ItemUnits;

	FString QuickSlotItem[MAX_QUICK_SLOT];

public:
	UMapInventory();
	virtual void SetOwner(IPlayerAbility* Target) override;
	virtual bool TryAddItem(const FString& ItemCode) override;
	virtual bool TryUseItem(const FString& ItemCode) override;
	
	virtual void QuickSlotAdd(const FString& ItemCode, const int Idx) override;
	virtual void UseItemAtQuickSlot(const int Idx) override;
	
	virtual TArray<InventoryItem> GetIter() override;

	
	void AddBullet(uint8 Count);
	uint8 GetBulletCount() const;

};
