// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemUI.h"

#include "Blueprint/WidgetTree.h"

/*
UInventoryItemUI::UInventoryItemUI()
{
}*/

void UInventoryItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Icon = Cast<UImage>(GetWidgetFromName("Icon"));
	DisplayText = Cast<UTextBlock>(GetWidgetFromName("DisplayText"));

	if (Icon == nullptr) UE_LOG(LogTemp, Warning, TEXT("NoIcon"));
	if (DisplayText == nullptr) UE_LOG(LogTemp, Warning, TEXT("NoText"));
	UE_LOG(LogTemp, Warning, TEXT("ITEM UI Set over"));

	FText test = FText::FromString("Hello world");
	DisplayText->SetText(test);
}

void UInventoryItemUI::SetUI(UTexture2D* ItemIcon, const FText& ItemName, uint8 Count = 0)
{
	UE_LOG(LogTemp, Warning, TEXT("Item: %s"), *ItemName.ToString());
	
	if (DisplayText == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoText"));
		return;
	}
	DisplayText->SetText(ItemName);
	if (Icon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoIcon"));
		return;
	}
	Icon->SetBrushFromTexture(ItemIcon);
}
