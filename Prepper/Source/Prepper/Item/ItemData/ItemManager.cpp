#include "ItemManager.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemCombinationData.h"
#include "UObject/ConstructorHelpers.h"
#include "ItemData.h"
#include "Prepper/_Base/DataTableGetter.h"

FString ItemManager::ItemCombineCode(const FString& Code1, const FString& Code2)
{
	if (Code1.Compare(Code2) > 0)
	{
		return Code1 + Code2;
	}

	return Code2 + Code1;
}

ItemManager::ItemManager()
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
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTableLoad: %d"), arr.Num());
		
	}
	
	UDataTable* ItemCombinationDataTable;
	
	if (DataTableGetter::GetDataTable("ItemCombinationDataTable", ItemCombinationDataTable))
	{
		TArray<FItemCombinationData*> arr;
		ItemCombinationDataTable->GetAllRows(TEXT("GetAllRows"), arr);
		
		for (int i = 0; i < arr.Num(); ++i)
		{;
			CombinationData.Add(ItemCombineCode(arr[i]->InputItemCode1, arr[i]->InputItemCode2), *arr[i]);
		}
		
	}
}

ItemManager::~ItemManager()
{
}

bool ItemManager::GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName)
{
	if (!ItemData.Contains(ItemCode)) return false;

	FItem* data = ItemData.Find(ItemCode);

	ItemIcon = data->ItemIcon;
	ItemName = data->ItemName;
	
	return true;
}
FItem* ItemManager::GetItem(const FString& ItemCode)
{
	if (!ItemData.Contains(ItemCode)) return nullptr;

	return ItemData.Find(ItemCode);
}

bool ItemManager::TryCombinationItem(const FString& ItemCode1, const FString& ItemCode2, FString& ResultCode)
{
	const FString CombinationCode = ItemCombineCode(ItemCode1, ItemCode2);
	
	if (!CombinationData.Contains(CombinationCode))
	{
		return false;
	}

	ResultCode = CombinationData.Find(CombinationCode)->OutputItemCode;

	return true;
}
