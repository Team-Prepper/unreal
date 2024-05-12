// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemUI.h"

#include "InventoryUI.h"
#include "ItemUIData.h"
#include "Blueprint/WidgetTree.h"
#include "Prepper/Interfaces/Inventory.h"

/*
UInventoryItemUI::UInventoryItemUI()
{
}*/

void UInventoryItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Icon = Cast<UImage>(GetWidgetFromName("Icon"));
	DisplayText = Cast<UTextBlock>(GetWidgetFromName("DisplayText"));

	if (Icon == nullptr) UE_LOG(LogTemp, Warning, TEXT("NoIcon"));
	if (DisplayText == nullptr) UE_LOG(LogTemp, Warning, TEXT("NoText"));
	UE_LOG(LogTemp, Warning, TEXT("ITEM UI Set over"));

	FText test = FText::FromString("Hello world");
	
	Icon->SetBrushFromTexture(ItemIcon);
	DisplayText->SetText(ItemText);
}

void UInventoryItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const UItemUIData* Object = Cast<UItemUIData>(ListItemObject);
	
	Icon->SetBrushFromTexture(Object->TextureIcon);
	DisplayText->SetText(Object->ItemName);
}

void UInventoryItemUI::SetUI(UTexture2D* TextureIcon, const FText& ItemName, uint8 Count = 0)
{
	UE_LOG(LogTemp, Warning, TEXT("Item: %s"), *ItemName.ToString());

	ItemIcon = TextureIcon;
	ItemText = ItemName;
}
