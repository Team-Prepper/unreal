// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prepper/Item/ItemDataGetter.h"
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
	
	ItemDataGetter ItemData;
	
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;
	
	IInventory* TargetInventory;
public:
	void Set(IInventory* TargetInventory);
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
protected:
	virtual void NativeOnInitialized() override;
};
