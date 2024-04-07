// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataGetter.h"

#include "ItemCombinationData.h"

FString ItemDataGetter::ItemCombineCode(const FString& Code1, const FString& Code2)
{
	if (Code1.Compare(Code2) > 0)
	{
		return Code1 + Code2;
	}

	return Code2 + Code1;
	
}

ItemDataGetter::ItemDataGetter()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
	ItemDataTable(TEXT("/Game/Data/ItemDataTable"));
	
	if (ItemDataTable.Succeeded())
	{
		TArray<FItemData*> arr;
		ItemDataTable.Object->GetAllRows(TEXT("GetAllRows"), arr);
		
		for (int i = 0; i < arr.Num(); ++i)
		{
			ItemData.Add(arr[i]->ItemCode, *arr[i]);
		}
		
	}
	
	static ConstructorHelpers::FObjectFinder<UDataTable> CombinationDataTable(
	TEXT("/Game/Data/ItemCombinationDataTable"));
	
	if (CombinationDataTable.Succeeded())
	{
		TArray<FItemCombinationData*> arr;
		CombinationDataTable.Object->GetAllRows(TEXT("GetAllRows"), arr);
		
		for (int i = 0; i < arr.Num(); ++i)
		{;
			CombinationData.Add(ItemCombineCode(arr[i]->InputItemCode1, arr[i]->InputItemCode2), *arr[i]);
		}
		
	}
}

ItemDataGetter::~ItemDataGetter()
{
    
}

bool ItemDataGetter::TryCombinationItem(const FString& ItemCode1, const FString& ItemCode2, FString& ResultCode)
{
	const FString CombinationCode = ItemCombineCode(ItemCode1, ItemCode2);
	
	if (!CombinationData.Contains(CombinationCode))
	{
		return false;
	}

	ResultCode = CombinationData.Find(CombinationCode)->OutputItemCode;

	return true;
}
