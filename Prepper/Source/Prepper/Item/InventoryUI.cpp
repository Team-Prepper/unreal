// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "InventoryItemUI.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Prepper/Interfaces/Inventory.h"

void UInventoryUI::Set(IInventory* Target)
{
	TargetInventory = Target;
}

void UInventoryUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if (InVisibility != ESlateVisibility::Visible) return;
	if (TargetInventory == nullptr) return;

	TArray<IInventory::InventoryItem> Items = TargetInventory->GetIter();
	
	for (int i = 0; i < Items.Num() && i < ItemList->GetNumItems(); i++)
	{
		UInventoryItemUI* ItemUI = Cast<UInventoryItemUI>(ItemList->GetItemAt(i));
		IInventory::InventoryItem Item = (Items)[i];
		UTexture2D* Icon;
		FText text;
		if (!ItemData.GetItemData(Item.ItemCode, Icon, text)) continue;
		
		ItemUI->SetUI(Icon, text, Item.count);
	}
}

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ItemList = Cast<UListView>(GetWidgetFromName(TEXT("ItemList")));
	for (int i = 0; i < 5; i++)
	{
		UInventoryItemUI* ItemUI = NewObject<UInventoryItemUI>(GetWorld(), ItemList->GetDefaultEntryClass());
		ItemList->AddItem(ItemUI);
	}
    
}