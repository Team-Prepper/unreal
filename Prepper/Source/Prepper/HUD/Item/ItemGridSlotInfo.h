// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemGridSlotInfo.generated.h"

class UImage;
class UTextBlock;
class UButton;
UCLASS()
class PREPPER_API UItemGridSlotInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	class IInventory* TargetInventory;
	FString SlotItemCode;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	UButton* UseButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuickSlotAddButton;
	UPROPERTY(meta = (BindWidget))
	UButton* DiscardButton;

public:
	virtual void NativeConstruct() override;
	
	void InspectItem(const FString& ItemCode);
	
	UFUNCTION()
	void ItemUse();
	UFUNCTION()
	void AddToQuickSlot();
};
