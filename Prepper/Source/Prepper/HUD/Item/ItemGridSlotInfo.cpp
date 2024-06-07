// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGridSlotInfo.h"
#include "ItemUIData.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Prepper/GameInstance/PrepperGameInstance.h"
#include "Prepper/Interfaces/Inventory.h"

void UItemGridSlotInfo::NativeConstruct()
{
	Super::NativeConstruct();

	UseButton->OnClicked.AddDynamic(this, &UItemGridSlotInfo::ItemUse);
	QuickSlotAddButton->OnClicked.AddDynamic(this, &UItemGridSlotInfo::AddToQuickSlot);
}

void UItemGridSlotInfo::InspectItem(const FString& ItemCode)
{
	UItemUIData* Data = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
	
	UPrepperGameInstance* PrepperGameInstance = Cast<UPrepperGameInstance>(GetGameInstance());
	if (PrepperGameInstance->ItemData.GetItemData(ItemCode, Data->TextureIcon, Data->ItemName))
	{
		ItemIcon->SetBrushFromTexture(Data->TextureIcon);
		ItemName->SetText(Data->ItemName);
		SlotItemCode = ItemCode;
	}
	
}

void UItemGridSlotInfo::ItemUse()
{
	if (TargetInventory == nullptr) return;
	TargetInventory->TryUseItem(SlotItemCode);
}

void UItemGridSlotInfo::AddToQuickSlot()
{
	if (TargetInventory == nullptr) return;
	TargetInventory->QuickSlotAdd(SlotItemCode, 0);
}
