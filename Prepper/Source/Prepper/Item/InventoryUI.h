// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

	UListView* ItemList;

public:
	void Set(IInventory& TargetInventory);
protected:
	virtual void NativeOnInitialized() override;
};
