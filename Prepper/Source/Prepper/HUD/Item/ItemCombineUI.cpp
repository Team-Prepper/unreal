// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombineUI.h"

#include "ItemUIData.h"
#include "Prepper/Item/ItemData/ItemManager.h"

void UItemCombineUI::UpdateData()
{
	if (TargetInventory == nullptr) return;

	TArray<IInventory::InventoryItem> Items = TargetInventory->GetIter();
	ItemList->ClearListItems();
	for (int i = 0; i < Items.Num(); i++)
	{
		UItemUIData* Data = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
		IInventory::InventoryItem Item = Items[i];
		
		if (!ItemManager::GetInstance()->GetItemData(Item.ItemCode, Data->TextureIcon, Data->ItemName)) continue;

		Data->ItemCount = Items[i].Count;
		Data->ItemCode = Items[i].ItemCode;
		
		ItemList->AddItem(Data);
	}
}

void UItemCombineUI::CombineButtonAction()
{
	if (Target1Code.Compare("") == 0 || Target2Code.Compare("") == 0) return;

	FString ItemCode;
	if (!ItemManager::GetInstance()->TryCombinationItem(Target1Code, Target2Code, ItemCode)) return;
	TargetInventory->TryAddItem(ItemCode);
	
}

void UItemCombineUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	UpdateData();
}

void UItemCombineUI::SetTargetInventory(IInventory* Target)
{
	TargetInventory = Target;
}

void UItemCombineUI::SetTargetItem(const FString& Target)
{
	if (!TargetInventory->TryUseItem(Target1Code)) return;
	
	if (Target1Code.Compare("") == 0)
	{
		Target1Code = Target;
		return;
	}
	if (Target2Code.Compare("") == 0)
	{
		Target2Code = Target;
		return;
	}

	TargetInventory->TryAddItem(Target1Code);
	Target1Code = Target;
}
