// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "InventoryItemUI.h"

void UInventoryUI::Set(IInventory& TargetInventory)
{
	ItemList->ClearListItems();
	for (int i = 0; i < 20; i++)
	{
		UInventoryItemUI* Item = Cast<UInventoryItemUI>(ItemList->GetItemAt(i));
		FText text = NSLOCTEXT("MyNamespace", "HelloWorld", "Hello World!");
		Item->SetUI(nullptr, text);
	}
}

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 위젯 블루프린트의 버튼을 이름을 통해 가져온다
	ItemList = Cast<UListView>(GetWidgetFromName(TEXT("ItemList")));
    
}