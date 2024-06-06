// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Prepper/Item/ItemData/ItemDataGetter.h"
#include "PrepperGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UPrepperGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ItemDataGetter ItemData;

public:
	FORCEINLINE const ItemDataGetter& GetItemData() const {return ItemData;}
	
};
