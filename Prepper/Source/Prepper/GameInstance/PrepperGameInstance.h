// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Prepper/Item/ItemData/ItemDataGetter.h"
#include "Prepper/_Base/UISystem/UIManager.h"
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

	virtual void Init() override {
		UIManager::Initialize();
	};	

	UPROPERTY()
	class UItemGridSlotInfo* ItemInfo;

public:
	FORCEINLINE const ItemDataGetter& GetItemData() const {return ItemData;}
	
};
