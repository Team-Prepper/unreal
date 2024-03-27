// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperPlayerController.h"

#include "EnhancedInputComponent.h"
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
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InPawn);
	TargetPlayer = Cast<IControllable>(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("This a testing statement. %s"), *Player->GetName())
	
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

void APrepperPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
	UE_LOG(LogTemp, Warning, TEXT("Binding"));
		// 점프는 추가해야 함
		// Moving
		///*
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::Look);
		//*/

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::SprintButtonPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APrepperPlayerController::SprintButtonReleased);

		// Equip
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::EquipButtonPressed);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::CrouchButtonPressed);

		//Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APrepperPlayerController::AimButtonReleased);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APrepperPlayerController::FireButtonReleased);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::ReloadButtonPressed);
	}
	
}
void APrepperPlayerController::Move(const FInputActionValue& Value)
{
	TargetPlayer = Cast<IControllable>(GetPawn());
	
	TargetPlayer->Move(Value);
}
void APrepperPlayerController::Look(const FInputActionValue& Value)
{
	if (!TargetPlayer) return;
	TargetPlayer->Look(Value);
}

void APrepperPlayerController::SprintButtonPressed()
{
	if (!TargetPlayer) return;
	TargetPlayer->ShiftPressed();
}

void APrepperPlayerController::SprintButtonReleased()
{
	if (!TargetPlayer) return;
	TargetPlayer->ShiftReleased();
}
void APrepperPlayerController::EquipButtonPressed()
{
	if (!TargetPlayer) return;
	TargetPlayer->EPressed();
	
}
void APrepperPlayerController::CrouchButtonPressed()
{
	if (!TargetPlayer) return;
	TargetPlayer->ControlPressed();
	
}
void APrepperPlayerController::ReloadButtonPressed()
{
	if (!TargetPlayer) return;
	TargetPlayer->RPressed();
}
void APrepperPlayerController::AimButtonPressed()
{
	
	if (!TargetPlayer) return;
	TargetPlayer->MouseRightPressed();
}
void APrepperPlayerController::AimButtonReleased()
{
	if (!TargetPlayer) return;
	TargetPlayer->MouseRightReleased();
	
}
void APrepperPlayerController::FireButtonPressed()
{
	if (!TargetPlayer) return;
	TargetPlayer->MouseLeftPressed();
	
}
void APrepperPlayerController::FireButtonReleased()
{
	
	if (!TargetPlayer) return;
	TargetPlayer->MouseLeftReleased();
	
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

