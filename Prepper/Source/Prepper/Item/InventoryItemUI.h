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
	//UPROPERTY(meta = (BindWidget))
	UImage* Icon;
	//UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayText;
public:
	void SetUI(UTexture2D* ItemIcon, const FText& ItemName);
protected:
	virtual void NativeOnInitialized() override;
};
