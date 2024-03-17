// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Prepper/HUD/CharacterOverlay.h"
#include "Prepper/HUD/PrepperHUD.h"



void APrepperPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrepperHUD = Cast<APrepperHUD>(GetHUD());
}

void APrepperPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->HealthBar &&
					 PrepperHUD->CharacterOverlay->HealthText;
	
	if(bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		PrepperHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		PrepperHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}

	
}