﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryQuickSlotUI.h"

#include "InventoryUIUnitData.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Item/ItemManager.h"

void UInventoryQuickSlotUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if (InVisibility == ESlateVisibility::Visible) return;

	MenuPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryQuickSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	QuickSlotRemoveButton->OnClicked.
			AddDynamic(this, &UInventoryQuickSlotUI::QuickSlotRemoveButtonAction);
	CancelButton->OnClicked.
			AddDynamic(this, &UInventoryQuickSlotUI::CancelButtonAction);
	MenuButton->OnClicked.
			AddDynamic(this, &UInventoryQuickSlotUI::MenuOpen);
}

void UInventoryQuickSlotUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	MenuPanel->SetVisibility(ESlateVisibility::Hidden);
	
	const TObjectPtr<UInventoryUIUnitData> Data
			= Cast<UInventoryUIUnitData>(ListItemObject);

	if (Data == nullptr) return;

	UTexture2D* Img;
	FText Name;
	TargetPlayer = Data->TargetPlayer;
	ItemCode = Data->ItemCode;
	Idx = Data->Idx;
	
	if (!ItemManager::GetInstance()->GetItemData(Data->ItemCode, Img, Name) || Data->ItemCount < 1)
	{
		DataExist = false;
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemIcon->SetOpacity(0.f);
		ItemCount->SetText(FText::FromString(FString("")));
		return;
	}

	DataExist = true;
	
	ItemIcon->SetOpacity(1.f);
	ItemIcon->SetBrushFromTexture(Img);
	ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Data->ItemCount)));
}

void UInventoryQuickSlotUI::QuickSlotRemoveButtonAction()
{
	IInventory* Inventory = TargetPlayer->GetInventory();

	Inventory->QuickSlotRemove(Idx);
	MenuPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryQuickSlotUI::CancelButtonAction()
{
	MenuPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryQuickSlotUI::MenuOpen()
{
	if (!DataExist) return;
	MenuPanel->SetVisibility(ESlateVisibility::Visible);
}
