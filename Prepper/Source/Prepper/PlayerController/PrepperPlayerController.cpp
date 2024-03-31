#include "PrepperPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/HUD/CharacterOverlay.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"
#include "Prepper/HUD/Announcement.h"


void APrepperPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrepperHUD = Cast<APrepperHUD>(GetHUD());

	if(PrepperHUD)
	{
		PrepperHUD->AddAnnouncement();
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}
}

void APrepperPlayerController::PollInit()
{
	if(CharacterOverlay == nullptr)
	{
		if(PrepperHUD && PrepperHUD->CharacterOverlay)
		{
			CharacterOverlay = PrepperHUD->CharacterOverlay;
			if(CharacterOverlay)
			{
				SetHUDHealth(HUDHealth,HUDMaxHealth);
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
	if(TargetPlayer == nullptr)
	{
		
		TargetPlayer = Cast<IControllable>(GetPawn());
	}
	
}

void APrepperPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InPawn);
	
	if (PlayerCharacter)
	{
		SetHUDHealth(PlayerCharacter->GetCurrentHealth(), PlayerCharacter->GetMaxHealth());
	}
}

void APrepperPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APrepperPlayerController,MatchState);
}

void APrepperPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
}

/* Input Binding */
void APrepperPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APrepperPlayerController::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrepperPlayerController::Look);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APrepperPlayerController::JumpButtonPressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APrepperPlayerController::JumpButtonReleased);

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
	if (!TargetPlayer) return;
	TargetPlayer->Move(Value);
}
void APrepperPlayerController::Look(const FInputActionValue& Value)
{
	if (!TargetPlayer) return;
	TargetPlayer->Look(Value);
}
void APrepperPlayerController::JumpButtonPressed()
{
	if (!TargetPlayer) return;
	TargetPlayer->SpacePressed();
}
void APrepperPlayerController::JumpButtonReleased()
{
	if (!TargetPlayer) return;
	TargetPlayer->SpaceReleased();
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

void APrepperPlayerController::BindPlayerAction()
{
	TargetPlayer = Cast<IControllable>(GetPawn());
}

/* HUD Setting*/
void APrepperPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountDown(MatchTime - GetServerTime());
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
	else
	{
		bInitCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
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
	else
	{
		bInitCharacterOverlay = true;
		HUDScore = Score;
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
	else
	{
		bInitCharacterOverlay = true;
		HUDDefeats = Defeats;
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

/* Sync time */
void APrepperPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void APrepperPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest,
	float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float APrepperPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void APrepperPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void APrepperPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void APrepperPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if(MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
}

void APrepperPlayerController::OnRep_MatchState()
{
	if(MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
}



void APrepperPlayerController::HandleMatchHasStarted()
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	if(PrepperHUD)
	{
		PrepperHUD->AddCharacterOverlay();
		if(PrepperHUD->Announcement)
		{
			PrepperHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

