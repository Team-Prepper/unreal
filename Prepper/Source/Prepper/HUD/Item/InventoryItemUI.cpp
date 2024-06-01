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
	
	UseButton->OnClicked.AddDynamic(this, &UInventoryItemUI::ItemUse);
}

void UInventoryItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const UItemUIData* Object = Cast<UItemUIData>(ListItemObject);

	ItemCode = Object->ItemCode;
	Icon->SetBrushFromTexture(Object->TextureIcon);
	DisplayText->SetText(Object->ItemName);
	ItemCount->SetText(FText::FromString(FString::FromInt(Object->ItemCount)));
	TargetInventoryUI = Object->TargetInventoryUI;
}

void UInventoryItemUI::ItemUse()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemUSE"));
	if (TargetInventoryUI == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("ItemUSE:%s"), *ItemCode);
	TargetInventoryUI->UseItem(ItemCode);
}
