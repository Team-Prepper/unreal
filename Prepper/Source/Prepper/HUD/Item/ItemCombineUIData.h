// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemCombineItemUI.h"
#include "ItemCombineUIData.generated.h"

UCLASS()
class PREPPER_API UItemCombineUIData : public UObject
{
	GENERATED_BODY()
public:
	UItemCombineUIData()
	{
		TargetInventory = nullptr;
		ActionUI = nullptr;
		
		TextureIcon = nullptr;
		ItemCode = "";
		ItemName = FText::FromString("");
		ItemCount = 0;
		
	}
	UItemCombineUIData(UItemCombineUI* Action, UTexture2D* Icon, const FString& Code, const FText& Name, int Count, IInventory* InventoryUI)
	{
		ActionUI = Action;
		TargetInventory = InventoryUI;
		
		TextureIcon = Icon;
		ItemCode = Code;
		ItemName = Name;
		ItemCount = Count;
	}

	IInventory* TargetInventory;
	UItemCombineUI* ActionUI;
	
	UTexture2D* TextureIcon;
	FString ItemCode;
	FText ItemName;
	uint8 ItemCount;
};
