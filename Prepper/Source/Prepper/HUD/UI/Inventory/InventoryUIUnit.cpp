// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUIUnit.h"

#include "InventoryUIUnitData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Prepper/Item/ItemManager.h"

void UInventoryUIUnit::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInventoryUIUnit::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	const UInventoryUIUnitData* Data = Cast<UInventoryUIUnitData>(ListItemObject);

	UTexture2D* Img;
	FText Name;
	
	if (!ItemManager::GetInstance()->GetItemData(Data->ItemCode, Img, Name)) return;

	ItemIcon->SetBrushFromTexture(Img);
	ItemName->SetText(Name);
}
