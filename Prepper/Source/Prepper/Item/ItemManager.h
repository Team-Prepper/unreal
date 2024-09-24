#pragma once

#include "ItemMeta.h"
#include "Prepper/Item/Item.h"
#include "ItemData/ItemCombinationData.h"
#include "Prepper/_Base/Singleton.h"

class ItemManager : public Singleton<ItemManager>
{
private:
	TMap<FString, FItem> ItemData;
	TMap<FString, FItemMeta> ItemMetaData;
	TMap<FString, FItemCombinationData> CombinationData;
	
	FString ItemCombineCode(const FString& Code1, const FString& Code2);
public:
	ItemManager();
	virtual ~ItemManager() override;
	void Initial(const TObjectPtr<UDataTable> ItemDataTable, const TObjectPtr<UDataTable> ItemCombinationDataTable);
	
	bool GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName);
	FItem* GetItem(const FString& ItemCode);
	FItemMeta* GetItemMeta(const FString& ItemCode);
	
	TObjectPtr<AActor> SpawnItem(UWorld* World, const FString& ItemCode);
	TObjectPtr<AInventoryInteractableItem> SpawnItemInteraction(UWorld* World, const FString& ItemCode);
	
	bool TryCombinationItem(const FString& ItemCode1, const FString& ItemCode2, FString& ResultCode);
	int CountCombinationData();
};
