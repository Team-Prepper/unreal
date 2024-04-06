#include "PrepperPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Component/CombatComponent.h"
#include "Prepper/HUD/CharacterOverlay.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"
#include "Prepper/GameState/DeathMatchGameState.h"
#include "Prepper/HUD/Announcement.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"


void APrepperPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PrepperHUD = Cast<APrepperHUD>(GetHUD());
	ServerCheckMatchState();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}
}


void APrepperPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APrepperPlayerController, MatchState);
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
	TargetPlayer = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Change Pawn"));
	
	if (PlayerCharacter)
	{
		SetHUDHealth(PlayerCharacter->GetCurrentHealth(), PlayerCharacter->GetMaxHealth());
	}
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
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if(HasAuthority())
	{
		DeathMatchGameMode = DeathMatchGameMode == nullptr ? Cast<ADeathMatchGameMode>(UGameplayStatics::GetGameMode(this)) : DeathMatchGameMode;
		if(DeathMatchGameMode)
		{
			SecondsLeft = FMath::CeilToInt(DeathMatchGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}
	
	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
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

void APrepperPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->WeaponAmmoValue;
	if (bHUDValid)
	{
		if(CountdownTime < 0.f)
		{
			PrepperHUD->CharacterOverlay->MatchCountDownText->SetText(FText());
			return;
		}
		
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountDownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PrepperHUD->CharacterOverlay->MatchCountDownText->SetText(FText::FromString(CountDownText));
	}
}

void APrepperPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->Announcement &&
					 PrepperHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if(CountdownTime < 0.f)
		{
			PrepperHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}
		
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PrepperHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
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
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void APrepperPlayerController::OnRep_MatchState()
{
	if(MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}



void APrepperPlayerController::HandleMatchHasStarted()
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	if(PrepperHUD)
	{
		if(PrepperHUD->CharacterOverlay == nullptr)
		PrepperHUD->AddCharacterOverlay();
		if(PrepperHUD->Announcement)
		{
			PrepperHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APrepperPlayerController::HandleCooldown()
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	if(PrepperHUD)
	{
		PrepperHUD->CharacterOverlay->RemoveFromParent();
		bool bHUDValid = PrepperHUD->Announcement &&
						 PrepperHUD->Announcement->AnnouncementText &&
						 PrepperHUD->Announcement->InfoText;
		if(bHUDValid)
		{
			PrepperHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText("New Match Starts In : ");
			PrepperHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));
			
			ADeathMatchGameState* DeathMatchGameState = Cast<ADeathMatchGameState>(UGameplayStatics::GetGameState(this));
			ADeathMatchPlayerState* DeathMatchPlayerState = GetPlayerState<ADeathMatchPlayerState>();
			if(DeathMatchGameState && DeathMatchPlayerState)
			{
				TArray<ADeathMatchPlayerState*> TopPlayers = DeathMatchGameState->TopScoringPlayers;
				FString InfoTexString;
				if(TopPlayers.Num() == 0)
				{
					InfoTexString = FString("There is no winner");
				}
				else if (TopPlayers.Num() == 1 && TopPlayers[0] == DeathMatchPlayerState)
				{
					InfoTexString = FString("You are the winner");
				}
				else if (TopPlayers.Num() == 1)
				{
					InfoTexString = FString::Printf(TEXT("Winner : \n%s"), *TopPlayers[0]->GetPlayerName());
				}
				else if (TopPlayers.Num() > 1)
				{
					InfoTexString = FString("Players tied for the win: \n");
					for (auto TiedPlayer : TopPlayers)
					{
						InfoTexString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
					}
				}
				
				PrepperHUD->Announcement->InfoText->SetText(FText::FromString(InfoTexString));
			}
			
		}
	}
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if(PlayerCharacter && PlayerCharacter->GetCombatComponent())
	{
		PlayerCharacter->bDisableGamePlay = true;
		PlayerCharacter->GetCombatComponent()->FireButtonPressed(false);
	}
}

void APrepperPlayerController::ServerCheckMatchState_Implementation()
{
	ADeathMatchGameMode* GameMode = Cast<ADeathMatchGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode)	
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void APrepperPlayerController::ClientJoinMidGame_Implementation(FName StateOfMatch, float Warmup, float Match,
	float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (PrepperHUD && MatchState == MatchState::WaitingToStart)
	{
		PrepperHUD->AddAnnouncement();
	}
}