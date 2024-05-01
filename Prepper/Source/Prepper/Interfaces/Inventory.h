// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventory : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IInventory
{
	GENERATED_BODY()
	
public:
	virtual bool TryAddItem(const FString& ItemCode) PURE_VIRTUAL(IIInventory::TryAddItem, return 0; ); 
	virtual bool TryUseItem(const FString& ItemCode) PURE_VIRTUAL(IIInventory::TryUseItem, return 0; );
};
