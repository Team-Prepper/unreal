// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorSettingUI.h"

#include "Components/Button.h"
#include "Prepper/GameMode/SurvivorGameMode.h"
#include "Prepper/PlayerController/BasePlayerController.h"

void USurvivorSettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SaveButton->OnClicked.
			AddDynamic(this, &USurvivorSettingUI::DataSave);
}

void USurvivorSettingUI::DataSave()
{
	TObjectPtr<ASurvivorGameMode> GM = GetWorld()->GetAuthGameMode<ASurvivorGameMode>();

	if (!GM) return;

	TObjectPtr<ABasePlayerController> PC = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	GM->SaveGame(PC->GetPlayerCharacter());
}
