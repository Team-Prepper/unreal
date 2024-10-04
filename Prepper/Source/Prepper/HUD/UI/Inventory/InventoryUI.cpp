// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "InventoryUIUnitData.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"
#include "Prepper/Item/Inventory/Inventory.h"

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CloseButton->OnClicked.AddDynamic(this, &UInventoryUI::Close);
}

void UInventoryUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC == nullptr) return;
	if (InVisibility == ESlateVisibility::Visible)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		SetFocus();
		return;
	}
	
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
	
}

void UInventoryUI::Update(IInventory* const& newData)
{
	InventoryView->ClearListItems();

	TArray<IInventory::InventoryItem> Items = newData->GetIter();
	
	for (int i = 0; i < Items.Num(); i++)
	{
		UInventoryUIUnitData* Data =
			NewObject<UInventoryUIUnitData>(GetWorld(), UInventoryUIUnitData::StaticClass());

		Data->ItemCode = Items[i].ItemCode;
		Data->ItemCount = Items[i].Count;
		
		InventoryView->AddItem(Data);
	}
	
	TArray<IInventory::InventoryItem> QuickSlots = newData->GetQuickSlotIter();
	
	for (int i = 0; i < QuickSlots.Num(); i++)
	{
		UInventoryUIUnitData* Data =
			NewObject<UInventoryUIUnitData>(GetWorld(), UInventoryUIUnitData::StaticClass());

		Data->ItemCode = QuickSlots[i].ItemCode;
		Data->ItemCount = QuickSlots[i].Count;
		
		QuickSlotView->AddItem(Data);
	}
}

void UInventoryUI::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}
