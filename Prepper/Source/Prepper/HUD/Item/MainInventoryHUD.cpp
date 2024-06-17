#include "MainInventoryHUD.h"
#include "ItemGrid.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Prepper/Interfaces/Inventory.h"

void UMainInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	ItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
	ItemGrid->MainHUD = this;
}

void UMainInventoryHUD::DecrementFText(FText& Text)
{
	// 1. FText를 FString으로 변환
	FString TextString = Text.ToString();

	// 2. FString을 int로 변환
	int32 Number = FCString::Atoi(*TextString);

	// 3. 값을 1 줄임
	Number--;

	// 4. 변경된 값을 다시 FString으로 변환
	FString NewTextString = FString::FromInt(Number);

	// 5. FString을 FText로 변환
	Text = FText::FromString(NewTextString);
}

void UMainInventoryHUD::ItemUse()
{
	if (TargetInventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("USE ITEM : NO INVETORY"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ITEM CODE : %s"),*SelectItemCode);
	TargetInventory->TryUseItem(SelectItemCode);
	ItemGrid->UpdateData();
	FText InfoCountText = InfoItemCountText->GetText();
	DecrementFText(InfoCountText);
	InfoItemCountText->SetText(InfoCountText);
}

void UMainInventoryHUD::AddToQuickSlot()
{
	if (TargetInventory == nullptr) return;
	TargetInventory->QuickSlotAdd(SelectItemCode, 0);
}

void UMainInventoryHUD::ItemDiscard()
{
	if (TargetInventory == nullptr) return;
	TargetInventory->TryDiscardItem(SelectItemCode);
	ItemGrid->UpdateData();
	FText InfoCountText = InfoItemCountText->GetText();
	DecrementFText(InfoCountText);
	InfoItemCountText->SetText(InfoCountText);
}

void UMainInventoryHUD::SetInventory(IInventory* Target)
{
	TargetInventory = Target;
	ItemGrid->Set(Target);
}

