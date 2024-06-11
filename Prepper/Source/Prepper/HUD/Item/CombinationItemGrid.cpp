#include "CombinationItemGrid.h"
#include "CombinationItemSlot.h"
#include "Components/UniformGridPanel.h"
#include "Prepper/Item/ItemData/ItemManager.h"

void UCombinationItemGrid::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("GRID INIT"));
}

void UCombinationItemGrid::AddSlot()
{
	GridSlots.Empty();
	GridPanel->ClearChildren();

	TArray<FString>Items;
	ItemManager::GetInstance()->CombinationResultToIngredients.GetKeys(Items);
	for(int i = 0; i < Items.Num(); i++)
	{
		UCombinationItemSlot* NewSlot = CreateWidget<UCombinationItemSlot>(this,ItemSlotClass);
		NewSlot->SetSlot(Items[i], CombineUI);
		UE_LOG(LogTemp, Warning, TEXT("ComItem Info : %s"),*Items[i]);
		GridSlots.Add(NewSlot);
		AddSlotsToGrid(i,NewSlot);
	}
	
}

void UCombinationItemGrid::AddSlotsToGrid(int Index, UCombinationItemSlot* NewSlot)
{
	int32 Row = Index / SlotsPerRow;
	int32 Col = Index % SlotsPerRow;
	GridPanel->AddChildToUniformGrid(NewSlot, Row,Col);
}
