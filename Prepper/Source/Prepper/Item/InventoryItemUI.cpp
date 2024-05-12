// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemUI.h"

/*
UInventoryItemUI::UInventoryItemUI()
{
}*/

void UInventoryItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 위젯 블루프린트의 버튼을 이름을 통해 가져온다
	Icon = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
    
	// 위젯 블루프린트의 텍스트 블록을 이름을 통해 가져온다
	DisplayText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
}

void UInventoryItemUI::SetUI(UTexture2D* ItemIcon, const FText& ItemName)
{
	Icon->SetBrushFromTexture(ItemIcon);
	DisplayText->SetText(ItemName);
}
