// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemCombinationUI.generated.h"

class APlayerCharacter;
class UItemCombinationSelectUI;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class PREPPER_API UItemCombinationUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CombinationButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CombinationResultIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CombinationSourceIcon1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CombinationSourceIcon2;
	
	UPROPERTY(EditAnywhere, Category="Select")
	TSubclassOf<UItemCombinationSelectUI> SelectWidget;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> TargetPlayer;
	FString TargetItem;
	FString SourceItem1;
	FString SourceItem2;

	int Cnt1;
	int Cnt2;
	
public:
	virtual void NativeOnInitialized() override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	void SetTargetPlayer(TObjectPtr<APlayerCharacter> Target);
	
	void SetCombinationTarget(const FString& CombinationTarget);
	UFUNCTION()
	void OpenSelectWidget();
	UFUNCTION()
	void Combination();
	UFUNCTION()
	void Close();
};
