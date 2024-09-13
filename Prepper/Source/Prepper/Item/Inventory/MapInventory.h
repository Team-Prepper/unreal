// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MAX_QUICK_SLOT 5

#include <set>

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Components/ActorComponent.h"
#include "Prepper/_Base/ObserverPattern/Subject.h"
#include "MapInventory.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UMapInventory : public UActorComponent, public IInventory, public ISubject<TMap<FString, uint8>>
{
	GENERATED_BODY()
private:
	IPlayerAbility * Owner;
	
	uint8 BulletCount;

	TMap<FString, uint8> ItemUnits;

	FString QuickSlotItem[MAX_QUICK_SLOT];

	UPROPERTY(EditAnywhere)
	uint8 InventorySize = 16;

private:
	std::pmr::set<IObserver<TMap<FString, uint8>>*> Observers;
	
public:
	UMapInventory();
	virtual bool TryAddItem(const FString& ItemCode) override;
	virtual bool TryUseItem(const FString& ItemCode) override;
	virtual bool TryDiscardItem(const FString& ItemCode) override;
	virtual int TryGetItemCount(const FString& ItemCode) override;
	virtual bool CheckOwnItem(const FString& ItemCode) override;
	
	virtual void QuickSlotAdd(const FString& ItemCode, const int Idx) override;
	virtual void UseItemAtQuickSlot(const int Idx) override;
	
	virtual TArray<InventoryItem> GetIter() const override;
	
	void AddBullet(uint8 Count);
	uint8 GetBulletCount() const;

	virtual void Attach(IObserver<TMap<FString, uint8>>* Observer) override;
	virtual void Detach(IObserver<TMap<FString, uint8>>* Observer) override;
	virtual void Notify() override;

};
