// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorController.h"

#include "EnhancedInputComponent.h"
#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/GameMode/SurvivorGameMode.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/HUD/UI/CharacterOverlay/StatusWidget.h"
#include "Prepper/HUD/UI/Inventory/InventoryUI.h"
#include "Prepper/HUD/UI/ItemCombination/ItemCombinationUI.h"

void ASurvivorController::BeginWidget()
{
	Super::BeginWidget();
	
	if (StatusWidgetClass && StatusWidget == nullptr)
	{
		StatusWidget = CreateWidget<UStatusWidget>(this, StatusWidgetClass);
		StatusWidget->AddToViewport();
	}
	
	if (InventoryWidgetClass && InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryUI>(this, InventoryWidgetClass);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->AddToViewport();
	}
}

void ASurvivorController::PossessPlayerCharacter()
{
	Super::PossessPlayerCharacter();
	
	if (!PlayerCharacter) return;

	if (ASurvivorGameMode* PrepperGameMode =
		GetWorld()->GetAuthGameMode<ASurvivorGameMode>(); PrepperGameMode != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Game"));
		PrepperGameMode->LoadGame(PlayerCharacter);
	}
	
	if (!CharacterOverlay) return;
	if (!StatusWidget) return;
	if (!PlayerCharacter->GetStatusEffectComponent()) return;

	UE_LOG(LogTemp, Warning, TEXT("Attach UI"));
	
	PlayerCharacter->GetStatusEffectComponent()->Attach(StatusWidget);
	PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
	InventoryWidget->SetTargetPlayer(PlayerCharacter);
}

void ASurvivorController::SetInput(UEnhancedInputComponent* Input)
{
	Super::SetInput(Input);
	
	Input->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &ASurvivorController::OpenInventoryPressed);
	Input->BindAction(Button1, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot1Use);
	Input->BindAction(Button2, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot2Use);
	Input->BindAction(Button3, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot3Use);
	Input->BindAction(Button4, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot4Use);
	Input->BindAction(Button5, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot5Use);
	
}

void ASurvivorController::OpenInventoryPressed()
{
	if (!InventoryWidget) return;
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	//ServerToggleInventory();
}

void ASurvivorController::ServerToggleInventory_Implementation()
{
	/*
	if (!TargetControllerable) return;
	TargetControllerable->ToggleInventory();
	*/
}

void ASurvivorController::QuickSlot1Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button1Pressed"));
	
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(0);
	
	return;
	if (ItemCombinationClass == nullptr) return;
	TObjectPtr<UItemCombinationUI> Tmp =
		CreateWidget<UItemCombinationUI>(this, ItemCombinationClass);
	
	Tmp->SetTargetPlayer(PlayerCharacter);
	Tmp->SetVisibility(ESlateVisibility::Visible);
	Tmp->AddToViewport();
}

void ASurvivorController::QuickSlot2Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button2Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(1);
}

void ASurvivorController::QuickSlot3Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button3Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(2);
}

void ASurvivorController::QuickSlot4Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button4Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(3);
}

void ASurvivorController::QuickSlot5Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button5Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(4);
}
