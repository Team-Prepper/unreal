// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameMode.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Prepper/GameState/DeathMatchGameState.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ADeathMatchGameMode::ADeathMatchGameMode()
{
	bDelayedStart = true;
}


void ADeathMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ADeathMatchGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APrepperPlayerController* PrepperController = Cast<APrepperPlayerController>(*It);
		if (PrepperController)
		{
			PrepperController->OnMatchStateSet(MatchState);
		}
	}
}

void ADeathMatchGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if(MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if(MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void ADeathMatchGameMode::PlayerEliminated(APlayerCharacter* ElimmedCharacter,
                                           APrepperPlayerController* VictimController, APrepperPlayerController* AttackerController)
{
	ADeathMatchPlayerState* AttackPlayerState = AttackerController ? Cast<ADeathMatchPlayerState>(AttackerController->PlayerState) : nullptr;
	ADeathMatchPlayerState* VictimPlayerState = VictimController ? Cast<ADeathMatchPlayerState>(VictimController -> PlayerState) : nullptr;

	ADeathMatchGameState* DeathMatchGameState = GetGameState<ADeathMatchGameState>();
	
	if(AttackPlayerState &&  AttackPlayerState != VictimPlayerState && DeathMatchGameState)
	{
		AttackPlayerState->AddToScore(1.0f);
		DeathMatchGameState->UpdateTopScore(AttackPlayerState);
	}
	if(VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void ADeathMatchGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
