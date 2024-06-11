// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Prepper/Interfaces/Inventory.h"
#include "ItemCombineUI.generated.h"


UCLASS()
class PREPPER_API UItemCombineUI : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UPROPERTY()
	class APlayerCharacter* InteractionPlayer;
	
	UPROPERTY()
	class UItemGrid* InvenItemGrid;
	UPROPERTY()
	class UCombinationItemGrid* CombinationItemGrid;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Target1Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Target1Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Target2Icon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Target2Name;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CombineButton;
	
	IInventory* TargetInventory;

	FString Target1Code;
	FString Target2Code;
	FString TargetCombinedCode;

	UFUNCTION(BlueprintCallable)
	void CombineButtonAction();

	virtual void NativeConstruct() override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	void SetTargetInventory(IInventory* Target);
	void SetTargetItem(const FString& Target);
	
};
