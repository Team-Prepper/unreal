// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInventoryHUD.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UMainInventoryHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UBorder* InventorySlot;

	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UUserWidget> ItemGridClass;

	UPROPERTY()
	class UItemGrid* ItemGrid;

	

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;
	
};
