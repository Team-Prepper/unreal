// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Prepper/GameState/DeathMatchGameState.h"
#include "Prepper/PlayerController/DeathMatchPlayerController.h"
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

	if(!GetWorld()->GetPlayerControllerIterator()) return;
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ADeathMatchPlayerController* PrepperController = Cast<ADeathMatchPlayerController>(*It);
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
		return;
	}
	if(MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
		return;
	}
	
	CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
	if(CountdownTime <= 0.f)
	{
		RestartGame();
	}
}

void ADeathMatchGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter,
                                           ABasePlayerController* VictimController, ABasePlayerController* AttackerController)
{
	/* for Score */
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

	RequestQueue.Add(FRequestQueueUnit(ElimmedCharacter, VictimController));
	
	FTimerHandle TestTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TestTimeHandle, this, &ADeathMatchGameMode::Respawn, RespawnTime);
	
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);

}

void ADeathMatchGameMode::Respawn()
{
	ADeathMatchGameMode* DeathMatchGameMode = GetWorld()->GetAuthGameMode<ADeathMatchGameMode>();

	if (!DeathMatchGameMode) return;
	
	DeathMatchGameMode->RequestRespawn(RequestQueue[0].Character, RequestQueue[0].Controller);
	RequestQueue.RemoveAt(0);
}

void ADeathMatchGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (!ElimmedController) return;
	
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	const int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
	RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	if(ABasePlayerController* ElimmedPlayerController = Cast<ABasePlayerController>(ElimmedController))
	{
		ElimmedPlayerController->SetPossessPawn();
	}
}
