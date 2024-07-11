#pragma once
#include "Item.h"
#include "CombinedItems.h"
#include "ItemCombinationData.h"
#include "Prepper/_Base/Singleton.h"

class ItemManager : public Singleton<ItemManager>
{
private:
	
	TMap<FString, FItem> ItemData;
	
	FString ItemCombineCode(const FString& Code1, const FString& Code2);
protected:
public:
	ItemManager();
	virtual ~ItemManager() override;

	TMap<FString, FItemCombinationData> CombinationData;
	TMap<FString, FCombinedItems> CombinationResultToIngredients;
	TMap<FString, TSubclassOf<class AInventoryInteractableItem>> InteractableItems;
	
	bool GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName);
	FItem* GetItem(const FString& ItemCode);
	bool TryCombinationItem(const FString& ItemCode1, const FString& ItemCode2, FString& ResultCode);
	int CountCombinationData();
};
