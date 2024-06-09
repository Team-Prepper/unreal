// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombineUI.h"

#include "ItemGrid.h"
#include "ItemUIData.h"
#include "Prepper/Item/ItemData/ItemManager.h"

void UItemCombineUI::NativeConstruct()
{
	Super::NativeConstruct();
	ItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
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
	if(!ItemGrid) return;
	ItemGrid->UpdateData();
}

void UItemCombineUI::SetTargetInventory(IInventory* Target)
{
	TargetInventory = Target;
	if(!ItemGrid) return;
	ItemGrid->Set(Target);
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
