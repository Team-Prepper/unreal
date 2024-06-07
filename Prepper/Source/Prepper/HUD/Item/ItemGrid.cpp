#include "ItemGrid.h"

#include "ItemGridSlot.h"
#include "ItemUIData.h"
#include "Components/UniformGridPanel.h"
#include "Prepper/GameInstance/PrepperGameInstance.h"


void UItemGrid::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("GRID INIT"));
	AddSlot(30);
}

void UItemGrid::Set(IInventory* Target)
{
	TargetInventory = Target;
	UpdateData();
}

void UItemGrid::AddSlot(int Capacity)
{
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
	if (TargetInventory == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("ui update"));
	UPrepperGameInstance* PrepperGameInstance = Cast<UPrepperGameInstance>(GetGameInstance());

	TArray<IInventory::InventoryItem> Items = TargetInventory->GetIter();
	for (int i = 0; i < Items.Num(); i++)
	{
		UItemUIData* Data = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
		IInventory::InventoryItem Item = Items[i];
		
		/* 아래부분 이해가 잘 안됨 */
		if (!PrepperGameInstance->ItemData.GetItemData(Item.ItemCode, Data->TextureIcon, Data->ItemName)) continue;
		Data->ItemCount = Items[i].Count;
		Data->ItemCode = Items[i].ItemCode;
		Data->TargetInventory = TargetInventory;

		//TODO
		GridSlots[i]->SetIcon(Data->TextureIcon, Data->ItemName, Data->ItemCount);
	}
}
