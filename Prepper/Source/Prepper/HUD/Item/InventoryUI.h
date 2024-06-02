// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prepper/Item/ItemData/ItemDataGetter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Prepper/Interfaces/Inventory.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;
	
	IInventory* TargetInventory;
	ItemDataGetter ItemData;
	
	void UpdateData();
public:
	void Set(IInventory * Target);
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	void UseItem(const FString& ItemCode);
protected:
	virtual void NativeOnInitialized() override;
};
