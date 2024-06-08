// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	void UpdateData();
	
public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	void Set(IInventory * Target);
	void UseItem(const FString& ItemCode);
};
