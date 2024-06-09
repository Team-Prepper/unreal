#include "ItemGrid.h"
#include "ItemGridSlot.h"
#include "ItemUIData.h"
#include "MainInventoryHUD.h"
#include "Components/UniformGridPanel.h"


void UItemGrid::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("GRID INIT"));
	AddSlot(InventoryCapacity);
}

void UItemGrid::Set(IInventory* Target)
{
	TargetInventory = Target;
	UpdateData();
}

void UItemGrid::AddSlot(int Capacity)
{
	GridSlots.Empty();
	GridPanel->ClearChildren();
	for(int i = 0; i < Capacity; i++)
	{
		UItemGridSlot* NewSlot = CreateWidget<UItemGridSlot>(this,ItemSlotClass);
		GridSlots.Add(NewSlot);
		AddSlotsToGrid(i,NewSlot);
	}
}

void UItemGrid::AddSlotsToGrid(int Index, UItemGridSlot* NewSlot)
{
	int32 Row = Index / SlotsPerRow;
	int32 Col = Index % SlotsPerRow;
	GridPanel->AddChildToUniformGrid(NewSlot, Row,Col);
}


void UItemGrid::UpdateData()
{
	GridSlots.Empty();
	GridPanel->ClearChildren();
	AddSlot(InventoryCapacity);
	if (TargetInventory == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("ui update"));
	TArray<IInventory::InventoryItem> Items = TargetInventory->GetIter();
	for (int i = 0; i < Items.Num(); i++)
	{
		UItemUIData* Data = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
		IInventory::InventoryItem Item = Items[i];

		GridSlots[i]->SetupSlot(Item.ItemCode, Items[i].Count);
		GridSlots[i]->SetInventory(TargetInventory);
		GridSlots[i]->MainHUD = MainHUD;
	}
}
