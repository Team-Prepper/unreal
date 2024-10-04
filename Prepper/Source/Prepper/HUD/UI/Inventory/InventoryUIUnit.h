// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUIUnit.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;

UCLASS()
class PREPPER_API UInventoryUIUnit : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
