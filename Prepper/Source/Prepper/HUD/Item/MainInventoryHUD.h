// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInventoryHUD.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class PREPPER_API UMainInventoryHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UItemGrid* ItemGrid;

	class IInventory* TargetInventory;
	FString SelectItemCode;

	/* Item Info */
	UPROPERTY(meta = (BindWidget))
	UImage* ItemInfoIcon;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemInfoName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemInfoDescription;
	
	UPROPERTY(meta = (BindWidget))
	UButton* UseButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuickSlotAddButton;
	UPROPERTY(meta = (BindWidget))
	UButton* DiscardButton;

	UFUNCTION()
	void ItemUse();
	UFUNCTION()
	void AddToQuickSlot();

	void SetInventory(IInventory* Target);
	
	virtual void NativeConstruct() override;
};
