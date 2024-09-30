// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorController.h"

#include "EnhancedInputComponent.h"
#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/HUD/UI/CharacterOverlay/StatusWidget.h"

void ASurvivorController::BeginWidget()
{
	Super::BeginWidget();
	
	if (StatusWidgetClass && StatusWidget == nullptr)
	{
		StatusWidget = CreateWidget<UStatusWidget>(this, StatusWidgetClass);
		StatusWidget->AddToViewport();
	}
}

void ASurvivorController::PossessPlayerCharacter()
{
	Super::PossessPlayerCharacter();
	
	if (!CharacterOverlay) return;
	if (!StatusWidget) return;
	if (!PlayerCharacter) return;
	if (!PlayerCharacter->GetStatusEffectComponent()) return;
	
	PlayerCharacter->GetStatusEffectComponent()->Attach(StatusWidget);
	PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
}

void ASurvivorController::SetInput(UEnhancedInputComponent* Input)
{
	Super::SetInput(Input);
	
	Input->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &ASurvivorController::OpenInventoryPressed);
	Input->BindAction(Button1, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot1Use);
	
}

void ASurvivorController::OpenInventoryPressed()
{
	ServerToggleInventory();
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
}