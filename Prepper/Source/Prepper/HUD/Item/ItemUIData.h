// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
		
		TextureIcon = nullptr;
		ItemName = FText::FromString("");
		
	}
	UItemUIData(UTexture2D* Icon, const FText& Name, int Count)
	{
		TextureIcon = Icon;
		ItemName = Name;
		ItemCount = Count;
	}

	UTexture2D* TextureIcon;
	FText ItemName;
	uint8 ItemCount;
};
