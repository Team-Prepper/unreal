#include "CombinationItemSlot.h"

#include "CraftUI.h"
#include "ItemUIData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Prepper/Item/ItemData/ItemManager.h"

void UCombinationItemSlot::SetSlot(const FString& CombinedItemCode, UCraftUI* CombineMainHUD)
{
	Data = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
	
	if(!ItemManager::GetInstance()->GetItemData(CombinedItemCode, Data->TextureIcon, Data->ItemName))
	{
		UE_LOG(LogTemp,Warning, TEXT("No Combine Item Data"));
		return;
	}
	ItemIcon->SetBrushFromTexture(Data->TextureIcon);
	ItemName->SetText(Data->ItemName);
	ItemCode = CombinedItemCode;
	CombineUI = CombineMainHUD;
}

void UCombinationItemSlot::SlotButtonPressed()
{
	if(!CombineUI)
	{
		UE_LOG(LogTemp,Warning, TEXT("No CombineHUD"));
		return;
	}
	CombineUI->TargetCombinedCode = ItemCode;
	FCombinedItems Ingredients = ItemManager::GetInstance()->CombinationResultToIngredients[ItemCode];
	UItemUIData* IngredientsData1 = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
	ItemManager::GetInstance()->GetItemData(Ingredients.CombinedItem1, IngredientsData1->TextureIcon, IngredientsData1->ItemName);
	CombineUI->Target1Icon->SetBrushFromTexture(IngredientsData1->TextureIcon);
	CombineUI->Target1Name->SetText(IngredientsData1->ItemName);
	CombineUI->Target1Code = Ingredients.CombinedItem1;
	UItemUIData* IngredientsData2 = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
	ItemManager::GetInstance()->GetItemData(Ingredients.CombinedItem2, IngredientsData2->TextureIcon, IngredientsData2->ItemName);
	CombineUI->Target2Icon->SetBrushFromTexture(IngredientsData2->TextureIcon);
	CombineUI->Target2Name->SetText(IngredientsData2->ItemName);
	CombineUI->Target2Code = Ingredients.CombinedItem2;
}
