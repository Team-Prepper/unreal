// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserListEntry.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryItemUI.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UInventoryItemUI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;
public:
	FText ItemText = FText::FromString("");
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
