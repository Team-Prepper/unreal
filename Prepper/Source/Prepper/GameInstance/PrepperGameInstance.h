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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TMap<FString, TSubclassOf<class AInventoryInteractableItem>> InteractableItems;


public:
	UPrepperGameInstance();
	
	TMap<FString, FItem> ItemData;

	virtual void Init() override {
		UIManager::Initialize();
	};	

public:
	bool GetItemData(const FString& ItemCode, UTexture2D*& ItemIcon, FText& ItemName);
	FItem* GetItem(const FString& ItemCode);
	TSubclassOf<class AInventoryInteractableItem>* GetItemInstance(FString ItemCode);
};
