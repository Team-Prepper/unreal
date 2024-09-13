// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemCombinationData.h"
#include "Prepper/Item/Item.h"

/**
 * 
 */
class PREPPER_API FItemDataGetter
{
private:
	TMap<FString, FItem> ItemData;
	TMap<FString, FItemCombinationData> CombinationData;
	FString ItemCombineCode(const FString& Code1, const FString& Code2);
public:
	FItemDataGetter();
	~FItemDataGetter();

	bool GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName);
	FItem* GetItem(const FString& ItemCode);
	
	bool TryCombinationItem(const FString& ItemCode1, const FString& ItemCode2, FString& ResultCode);
};
