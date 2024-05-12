// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

UCLASS()
class PREPPER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefeatsValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponAmmoValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CarriedAmmoValue;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MatchCountDownText;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* HighPingImg;

	UPROPERTY(meta = (BindWidget))
	class UUserWidget* InventoryHUD;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HighPingAnim;

	UFUNCTION()
	void SetInventoryVisible(bool IsVisible);
	
	void SetInventory(IInventory *Target);
	
};
