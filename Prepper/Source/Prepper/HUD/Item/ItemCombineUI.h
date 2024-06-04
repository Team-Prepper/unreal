// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Prepper/Interfaces/Inventory.h"
#include "Prepper/Item/ItemData/ItemDataGetter.h"
#include "ItemCombineUI.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UItemCombineUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UListView* ItemList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Target1Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Target1Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Target2Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Target2Name;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CombineButton;
	
	ItemDataGetter ItemData;
	IInventory* TargetInventory;

	FString Target1Code;
	FString Target2Code;

	void UpdateData();

	UFUNCTION()
	void CombineButtonAction();

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	void SetTargetInventory(IInventory* Target);
	void SetTargetItem(const FString& Target);
	
};
