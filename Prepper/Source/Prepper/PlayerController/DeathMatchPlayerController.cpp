#include "DeathMatchPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Prepper/Character/Component/Combat/CombatComponent.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"
#include "Prepper/GameState/DeathMatchGameState.h"
#include "Prepper/HUD/UI/Announcement.h"
#include "Prepper/HUD/UI/CharacterOverlay/CharacterOverlay.h"
#include "Prepper/HUD/UI/DeathMatch/ScoreBoard.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"

void ADeathMatchPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ServerCheckMatchState();
}

void ADeathMatchPlayerController::PollInit()
{
	Super::PollInit();
	
	if (ScoreBoardClass && ScoreBoard == nullptr)
	{
		ScoreBoard = CreateWidget<UScoreBoard>(this, ScoreBoardClass);
		ScoreBoard->AddToViewport();
		ScoreBoard->SetVisibility(ESlateVisibility::Hidden);
	}

	if (PrepperHUD->CharacterOverlay)
	{
		PrepperHUD->CharacterOverlay->ToggleDeathMatch(true);
		PrepperHUD->CharacterOverlay->ToggleStory(false);
	}
	
	SetHUDScore(HUDScore);
	SetHUDDefeats(HUDDefeats);

	ADeathMatchGameState* DeathMatchGameState =
		Cast<ADeathMatchGameState>(UGameplayStatics::GetGameState(this));
	ADeathMatchPlayerState* DeathMatchPlayerState = GetPlayerState<ADeathMatchPlayerState>();

	if (!DeathMatchGameState || !DeathMatchPlayerState) return;
	
	DeathMatchGameState->UpdateTopScore(DeathMatchPlayerState);
}

void ADeathMatchPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
	CheckTimeSync(DeltaTime);
}

void ADeathMatchPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADeathMatchPlayerController, MatchState);
}

void ADeathMatchPlayerController::SetHUDScore(float Score)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	if (PrepperHUD && PrepperHUD->CharacterOverlay)
	{
		PrepperHUD->CharacterOverlay->SetScore(Score);
	}
	else
	{
		HUDScore = Score;
	}
}

void ADeathMatchPlayerController::SetHUDDefeats(int32 Defeats)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	if (PrepperHUD && PrepperHUD->CharacterOverlay)
	{
		PrepperHUD->CharacterOverlay->SetDefeat(Defeats);
	}
	else
	{
		HUDDefeats = Defeats;
	}
}

void ADeathMatchPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;

	if (PrepperHUD && PrepperHUD->CharacterOverlay)
	{
		if (CountdownTime < 0.f)
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

void ADeathMatchPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
		PrepperHUD->Announcement &&
		PrepperHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
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

void ADeathMatchPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress)
		TimeLeft = WarmupTime + MatchTime - GetServerTime() +
			LevelStartingTime;
	else if (MatchState == MatchState::Cooldown)
		TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() +
			LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		DeathMatchGameMode = DeathMatchGameMode == nullptr
			                     ? Cast<ADeathMatchGameMode>(UGameplayStatics::GetGameMode(this))
			                     : DeathMatchGameMode;
		if (DeathMatchGameMode)
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

void ADeathMatchPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

float ADeathMatchPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ADeathMatchPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ADeathMatchPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void ADeathMatchPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void ADeathMatchPlayerController::HandleMatchHasStarted()
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	if (PrepperHUD)
	{
		if (PrepperHUD->Announcement)
		{
			PrepperHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ADeathMatchPlayerController::HandleCooldown()
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter && PlayerCharacter->GetCombatComponent())
	{
		PlayerCharacter->bDisableGamePlay = true;
		PlayerCharacter->GetCombatComponent()->FireTrigger(false);
	}
	
	if (!PrepperHUD) return;
	
	PrepperHUD->CharacterOverlay->RemoveFromParent();
	bool bHUDValid = PrepperHUD->Announcement &&
		PrepperHUD->Announcement->AnnouncementText &&
		PrepperHUD->Announcement->InfoText;
	
	if (!bHUDValid) return;
	
	PrepperHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
	PrepperHUD->Announcement->AnnouncementText->SetText(FText::FromString(FString("New Match Starts In : ")));

	ADeathMatchGameState* DeathMatchGameState = Cast<
		ADeathMatchGameState>(UGameplayStatics::GetGameState(this));
	const ADeathMatchPlayerState* DeathMatchPlayerState = GetPlayerState<ADeathMatchPlayerState>();
		
	if (!DeathMatchGameState || !DeathMatchPlayerState) return;

	const FString InfoTexString = DeathMatchGameState->GetMatchResultStr(DeathMatchPlayerState);
	PrepperHUD->Announcement->InfoText->SetText(FText::FromString(InfoTexString));
}

void ADeathMatchPlayerController::ClientJoinMidGame_Implementation(FName StateOfMatch, float Warmup, float Match,
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

void ADeathMatchPlayerController::ServerCheckMatchState_Implementation()
{
	ADeathMatchGameMode* GameMode = Cast<ADeathMatchGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void ADeathMatchPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest,
                                                                        float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void ADeathMatchPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ADeathMatchPlayerController::SetInput(UEnhancedInputComponent* Input)
{
	Super::SetInput(Input);
	Input->BindAction(OpenInventory, ETriggerEvent::Started, this, &ADeathMatchPlayerController::OpenScoreBoard);
	Input->BindAction(OpenInventory, ETriggerEvent::Completed, this, &ADeathMatchPlayerController::CloseScoreBoard);
}

void ADeathMatchPlayerController::OpenScoreBoard()
{
	if (!ScoreBoard)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO SCOREBOARD"));
		return;
	}
	ScoreBoard->SetVisibility(ESlateVisibility::Visible);
}

void ADeathMatchPlayerController::CloseScoreBoard()
{
	if (!ScoreBoard)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO SCOREBOARD"));
		return;
	}
	ScoreBoard->SetVisibility(ESlateVisibility::Hidden);
}
