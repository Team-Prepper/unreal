// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/HUD/PrepperHUD.h"

void AStoryPlayerController::PollInit()
{
	Super::PollInit();
	
	PrepperHUD->CharacterOverlay->ToggleDeathMatch(true);
	PrepperHUD->CharacterOverlay->ToggleStory(false);
}

void AStoryPlayerController::PossessPlayerCharacter()
{
	Super::PossessPlayerCharacter();
	
	if(PlayerCharacter && PlayerCharacter->GetStatusEffectComponent())
	{
		PlayerCharacter->GetStatusEffectComponent()->Attach(PrepperHUD->CharacterOverlay);
		PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
	}
}

void AStoryPlayerController::SetInput(UEnhancedInputComponent* Input)
{
	Super::SetInput(Input);
	
	Input->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &AStoryPlayerController::OpenInventoryPressed);
	Input->BindAction(Button1, ETriggerEvent::Started, this, &AStoryPlayerController::QuickSlot1Use);
	
}

void AStoryPlayerController::OpenInventoryPressed()
{
	ServerToggleInventory();
}

void AStoryPlayerController::ServerToggleInventory_Implementation()
{
	if (!TargetControllerable) return;
	TargetControllerable->ToggleInventory();
}

void AStoryPlayerController::QuickSlot1Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button1Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(0);
}