// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftUI.h"

#include "ItemGrid.h"
#include "ItemUIData.h"
#include "Prepper/Item/ItemData/ItemManager.h"

void UCraftUI::NativeConstruct()
{
	Super::NativeConstruct();
	ItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
	
	CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UGUIFullScreenWidget::Close);
	}
}

void UCraftUI::CombineButtonAction()
{
	if (Target1Code.Compare("") == 0 || Target2Code.Compare("") == 0) return;

	FString ItemCode;
	if (!ItemManager::GetInstance()->TryCombinationItem(Target1Code, Target2Code, ItemCode)) return;
	TargetInventory->TryAddItem(ItemCode);
	ItemGrid->UpdateData();
}

void UCraftUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if(!ItemGrid) return;
	ItemGrid->UpdateData();
}

void UCraftUI::Open()
{
	Super::Open();
	GetOwningPlayer()->SetShowMouseCursor(true);
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
}

void UCraftUI::Close()
{
	Super::Close();
	GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
}

void UCraftUI::SetTargetInventory(IInventory* Target)
{
	TargetInventory = Target;
	if(!ItemGrid) return;
	ItemGrid->Set(Target);
}

void UCraftUI::SetTargetItem(const FString& Target)
{
	if (!TargetInventory->TryUseItem(Target1Code)) return;
	
	if (Target1Code.Compare("") == 0)
	{
		Target1Code = Target;
		ItemGrid->UpdateData();
		return;
	}
	if (Target2Code.Compare("") == 0)
	{
		Target2Code = Target;
		ItemGrid->UpdateData();
		return;
	}

	TargetInventory->TryAddItem(Target1Code);
	Target1Code = Target;
	ItemGrid->UpdateData();
}
