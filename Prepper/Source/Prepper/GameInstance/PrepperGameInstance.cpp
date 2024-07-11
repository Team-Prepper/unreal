// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperGameInstance.h"
#include "Prepper/Item/ItemData/ItemData.h"
#include "Prepper/_Base/DataTableGetter.h"

namespace EQSDebug
{
	struct FItemData;
}

UPrepperGameInstance::UPrepperGameInstance()
{
	UDataTable* ItemDataTable;
	
	if (DataTableGetter::GetDataTable("ItemDataTable", ItemDataTable))
	{
		TArray<FItemData*> arr;
		ItemDataTable->GetAllRows(TEXT("GetAllRows"), arr);
		
		for (int i = 0; i < arr.Num(); ++i)
		{
			ItemData.Add(arr[i]->ItemCode, arr[i]->GetItem());
		}
		UE_LOG(LogTemp, Warning, TEXT("GameInstance : ItemDataTableLoad: %d"), arr.Num());
	}
}

bool UPrepperGameInstance::GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName)
{
	if (!ItemData.Contains(ItemCode)) return false;

	FItem* data = ItemData.Find(ItemCode);

	ItemIcon = data->ItemIcon;
	ItemName = data->ItemName;
	
	return true;
}

FItem* UPrepperGameInstance::GetItem(const FString& ItemCode)
{
	if (!ItemData.Contains(ItemCode)) return nullptr;

	return ItemData.Find(ItemCode);
}

TSubclassOf<AInventoryInteractableItem>* UPrepperGameInstance::GetItemInstance(FString ItemCode)
{
	if(!InteractableItems.Contains(ItemCode)) return nullptr;
	return InteractableItems.Find(ItemCode);
}
