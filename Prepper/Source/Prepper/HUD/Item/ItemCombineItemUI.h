// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemCombineUI.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Prepper/Interfaces/Inventory.h"
#include "ItemCombineItemUI.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UItemCombineItemUI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
private:
	IInventory* TargetInventory;
	UItemCombineUI* ActionUI;
	FString ItemCode;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;
	
public:
	FText ItemText = FText::FromString("");
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void Select();
	
};
