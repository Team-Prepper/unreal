// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Prepper/Interfaces/Inventory.h"
#include "Prepper/_Base/UISystem/GUIFullScreen.h"
#include "Prepper/_Base/UISystem/GUIFullScreenWidget.h"
#include "CraftUI.generated.h"

UCLASS()
class PREPPER_API UCraftUI : public UGUIFullScreenWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	class UItemGrid* ItemGrid;
	
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
	
	TObjectPtr<UButton> CloseButton;
	
	IInventory* TargetInventory;

	FString Target1Code;
	FString Target2Code;

	
	UFUNCTION()
	void CombineButtonAction();

public:
	virtual void NativeConstruct() override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	virtual void Open() override;
	virtual void Close() override;
	void SetTargetInventory(IInventory* Target);
	void SetTargetItem(const FString& Target);
	
};
