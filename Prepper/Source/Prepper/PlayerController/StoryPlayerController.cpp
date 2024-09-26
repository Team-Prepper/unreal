// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryPlayerController.h"

#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/HUD/PrepperHUD.h"

void AStoryPlayerController::PossessPlayerCharacter()
{
	Super::PossessPlayerCharacter();
	
	if(PlayerCharacter && PlayerCharacter->GetStatusEffectComponent())
	{
		PlayerCharacter->GetStatusEffectComponent()->Attach(PrepperHUD->CharacterOverlay);
		PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
	}
}
