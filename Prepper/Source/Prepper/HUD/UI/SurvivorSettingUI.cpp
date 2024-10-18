// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorSettingUI.h"

#include "Components/Button.h"
#include "Prepper/PlayerController/SurvivorController.h"

void USurvivorSettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SaveButton->OnClicked.
			AddDynamic(this, &USurvivorSettingUI::DataSave);
}

void USurvivorSettingUI::DataSave()
{

	TObjectPtr<ASurvivorController> PC = Cast<ASurvivorController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr) return;

	PC->SaveGame();
}
