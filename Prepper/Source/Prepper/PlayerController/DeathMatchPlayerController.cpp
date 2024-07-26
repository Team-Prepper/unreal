#include "DeathMatchPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Prepper/Component/CombatComponent.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"
#include "Prepper/GameState/DeathMatchGameState.h"
#include "Prepper/HUD/UI/Announcement.h"
#include "Prepper/HUD/UI/CharacterOverlay/CharacterOverlay.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"

void ADeathMatchPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ServerCheckMatchState();
}

void ADeathMatchPlayerController::PollInit()
{
	Super::PollInit();
	if(CharacterOverlay == nullptr)
	{
		if(PrepperHUD && PrepperHUD->CharacterOverlay)
		{
			CharacterOverlay = PrepperHUD->CharacterOverlay;
			if(CharacterOverlay)
			{
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
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

void ADeathMatchPlayerController::SetHUDDefeats(int32 Defeats)
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

void ADeathMatchPlayerController::SetHUDMatchCountdown(float CountdownTime)
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

void ADeathMatchPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
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

void ADeathMatchPlayerController::SetHUDTime()
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
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
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

	if(MatchState == MatchState::InProgress)
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
	if(MatchState == MatchState::InProgress)
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

void ADeathMatchPlayerController::HandleCooldown()
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
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if(PlayerCharacter && PlayerCharacter->GetCombatComponent())
	{
		PlayerCharacter->bDisableGamePlay = true;
		PlayerCharacter->GetCombatComponent()->FireTrigger(false);
	}
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