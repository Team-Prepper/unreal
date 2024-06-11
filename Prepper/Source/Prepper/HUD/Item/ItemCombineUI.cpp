// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombineUI.h"

#include "CombinationItemGrid.h"
#include "ItemGrid.h"
#include "ItemUIData.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Item/ItemData/ItemManager.h"

void UItemCombineUI::NativeConstruct()
{
	Super::NativeConstruct();
	InvenItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
	CombinationItemGrid = Cast<UCombinationItemGrid>(GetWidgetFromName("WBP_CombinationGrid"));
	if(CombinationItemGrid)
	{
		CombinationItemGrid->CombineUI = this;
		UE_LOG(LogTemp,Warning, TEXT("Crafting->Grid success"));
	}
	
}

void UItemCombineUI::CombineButtonAction()
{
	/*
	if (Target1Code.Compare("") == 0 || Target2Code.Compare("") == 0) return;

	FString ItemCode;
	if (!ItemManager::GetInstance()->TryCombinationItem(Target1Code, Target2Code, ItemCode)) return;
	TargetInventory->TryAddItem(ItemCode);
	*/
	UE_LOG(LogTemp, Warning, TEXT("TargetItem1 : %s"),*Target1Code);
	UE_LOG(LogTemp, Warning, TEXT("TargetItem2 : %s"),*Target2Code);

	if(!InteractionPlayer->Inven->CheckOwnItem(Target1Code))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetItem1 : %s NO Item"),*Target1Code);
		return;
	}
	if(!InteractionPlayer->Inven->CheckOwnItem(Target2Code))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetItem2 : %s NO Item"),*Target2Code);
		return;
	}
	if(!InteractionPlayer->Inven->TryDiscardItem(Target1Code))
	{
		UE_LOG(LogTemp, Warning, TEXT("BUG REPORT : Can not combine"));
		return;
	}
	if(!InteractionPlayer->Inven->TryDiscardItem(Target2Code))
	{
		UE_LOG(LogTemp, Warning, TEXT("BUG REPORT : Can not combine"));
		if(!TargetInventory->TryAddItem(Target1Code))
		{
			UE_LOG(LogTemp, Warning, TEXT("DANGER BUG REPORT : %s ITEM ERROR"),*Target1Code);
		}
		return;
	}
	TargetInventory->TryAddItem(TargetCombinedCode);
	InvenItemGrid->UpdateData();
}



void UItemCombineUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if(!InvenItemGrid) return;
	InvenItemGrid->UpdateData();
	CombinationItemGrid->AddSlot();
}

void UItemCombineUI::SetTargetInventory(IInventory* Target)
{
	TargetInventory = Target;
	if(!InvenItemGrid) return;
	InvenItemGrid->Set(Target);
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
