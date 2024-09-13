// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperGameInstance.h"

#include "Prepper/Item/ItemManager.h"
#include "Prepper/Item/ItemData/ItemData.h"
#include "Prepper/_Base/DataTableGetter.h"

namespace EQSDebug
{
	struct FItemData;
}

UPrepperGameInstance::UPrepperGameInstance()
{
	DataTableGetter::GetDataTable("ItemDataTable", ItemDataTable);
	DataTableGetter::GetDataTable("ItemCombinationDataTable", ItemCombinationDataTable);

}

void UPrepperGameInstance::Init()
{
	Super::Init();
	UIManager::Initialize();
	ItemManager::GetInstance()->Initial(ItemDataTable, ItemCombinationDataTable);
	
};
