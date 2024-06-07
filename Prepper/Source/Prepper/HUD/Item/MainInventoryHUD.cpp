#include "MainInventoryHUD.h"
#include "ItemGrid.h"
#include "Components/Button.h"
#include "Prepper/Interfaces/Inventory.h"

void UMainInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	ItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
	ItemGrid->MainHUD = this;
	
	UseButton->OnClicked.AddDynamic(this, &UMainInventoryHUD::ItemUse);
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
}

void UMainInventoryHUD::AddToQuickSlot()
{
	if (TargetInventory == nullptr) return;
	TargetInventory->QuickSlotAdd(SelectItemCode, 0);
}

void UMainInventoryHUD::SetInventory(IInventory* Target)
{
	TargetInventory = Target;
	ItemGrid->Set(Target);
}

