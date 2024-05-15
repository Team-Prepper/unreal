// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemUI.h"

#include "InventoryUI.h"
#include "ItemUIData.h"

/*
UInventoryItemUI::UInventoryItemUI()
{
}*/

void UInventoryItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Icon = Cast<UImage>(GetWidgetFromName("Icon"));
	DisplayText = Cast<UTextBlock>(GetWidgetFromName("DisplayText"));
}

void UInventoryItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const UItemUIData* Object = Cast<UItemUIData>(ListItemObject);
	
	Icon->SetBrushFromTexture(Object->TextureIcon);
	DisplayText->SetText(Object->ItemName);
}