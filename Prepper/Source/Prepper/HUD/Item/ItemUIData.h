// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemUIData.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UItemUIData : public UObject
{
	GENERATED_BODY()
	
public:
	UItemUIData()
	{
		TargetInventoryUI = nullptr;
		
		TextureIcon = nullptr;
		ItemCode = "";
		ItemName = FText::FromString("");
		ItemCount = 0;
		
	}
	UItemUIData(UTexture2D* Icon, const FString& Code, const FText& Name, int Count, UInventoryUI* InventoryUI)
	{
		TargetInventoryUI = InventoryUI;
		
		TextureIcon = Icon;
		ItemCode = Code;
		ItemName = Name;
		ItemCount = Count;
	}

	UInventoryUI* TargetInventoryUI;
	
	UTexture2D* TextureIcon;
	FString ItemCode;
	FText ItemName;
	uint8 ItemCount;
};
