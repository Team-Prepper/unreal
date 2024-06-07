// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombineItemUI.h"

#include "ItemCombineUIData.h"

void UItemCombineItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UItemCombineItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	const UItemCombineUIData* Object = Cast<UItemCombineUIData>(ListItemObject);
	
	ItemCode = Object->ItemCode;
	Icon->SetBrushFromTexture(Object->TextureIcon);
	DisplayText->SetText(Object->ItemName);
	ItemCount->SetText(FText::FromString(FString::FromInt(Object->ItemCount)));
	TargetInventory = Object->TargetInventory;
	ActionUI = Object->ActionUI;
}

void UItemCombineItemUI::Select()
{
	ActionUI->SetTargetItem(ItemCode);
}
