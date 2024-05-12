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
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Icon;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> DisplayText;
public:
	void SetUI(UTexture2D* ItemIcon, const FText& ItemName, uint8 Count);
protected:
	virtual void NativeOnInitialized() override;
};
