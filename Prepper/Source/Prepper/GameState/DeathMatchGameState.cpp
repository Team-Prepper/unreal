// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameState.h"

#include "Net/UnrealNetwork.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"

void ADeathMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathMatchGameState, TopScoringPlayers);
}

void ADeathMatchGameState::UpdateTopScore(ADeathMatchPlayerState* ScoringPlayer)
{
	if(TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if(ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}
