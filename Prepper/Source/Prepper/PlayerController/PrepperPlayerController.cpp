// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/HUD/CharacterOverlay.h"
#include "Prepper/HUD/PrepperHUD.h"



void APrepperPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrepperHUD = Cast<APrepperHUD>(GetHUD());
}

void APrepperPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 리스폰시 플레이어 HUD 동기화
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InPawn);
	if (PlayerCharacter)
	{
		SetHUDHealth(PlayerCharacter->GetCurrentHealth(), PlayerCharacter->GetMaxHealth());
	}
}

void APrepperPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
}

void APrepperPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountDown(MatchTime - GetWorld()->GetTimeSeconds());
	}

	CountdownInt = SecondsLeft;
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

void APrepperPlayerController::SetHUDScore(float Score)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->ScoreValue;
	if(bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"),FMath::FloorToInt(Score));
		PrepperHUD->CharacterOverlay->ScoreValue->SetText(FText::FromString(ScoreText));
	}
}

void APrepperPlayerController::SetHUDDefeats(int32 Defeats)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->DefeatsValue;
	if(bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"),Defeats);
		PrepperHUD->CharacterOverlay->DefeatsValue->SetText(FText::FromString(DefeatsText));
	}
}

void APrepperPlayerController::SetHUDWeaponAmmo(int32 Value)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->WeaponAmmoValue;
	if(bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"),Value);
		PrepperHUD->CharacterOverlay->WeaponAmmoValue->SetText(FText::FromString(AmmoText));
	}
}

void APrepperPlayerController::SetHUDCarriedAmmo(int32 Value)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->CarriedAmmoValue;
	if(bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"),Value);
		PrepperHUD->CharacterOverlay->CarriedAmmoValue->SetText(FText::FromString(AmmoText));
	}
}

void APrepperPlayerController::SetHUDMatchCountDown(float CountDownTime)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->WeaponAmmoValue;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountDownTime / 60.f);
		int32 Seconds = CountDownTime - Minutes * 60;

		FString CountDownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PrepperHUD->CharacterOverlay->MatchCountDownText->SetText(FText::FromString(CountDownText));
	}
}

