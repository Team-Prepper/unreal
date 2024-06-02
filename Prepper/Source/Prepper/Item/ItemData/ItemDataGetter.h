// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemData.h"
#include "ItemCombinationData.h"

/**
 * 
 */
class PREPPER_API ItemDataGetter
{
private:
	
	TMap<FString, FItem> ItemData;
	TMap<FString, FItemCombinationData> CombinationData;
	FString ItemCombineCode(const FString& Code1, const FString& Code2);
public:
	ItemDataGetter();
	~ItemDataGetter();

	bool GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName);
	FItem* GetItem(const FString& ItemCode);
	bool TryCombinationItem(const FString& ItemCode1, const FString& ItemCode2, FString& ResultCode);
};
