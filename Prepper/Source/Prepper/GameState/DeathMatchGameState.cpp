// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameState.h"

#include "Net/UnrealNetwork.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"

void ADeathMatchGameState::OnRep_Player()
{
	Notify();
}

void ADeathMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathMatchGameState, Players);
	DOREPLIFETIME(ADeathMatchGameState, TopScoringPlayers);
}

void ADeathMatchGameState::UpdateTopScore(ADeathMatchPlayerState* ScoringPlayer)
{
	if (!Players.Contains(ScoringPlayer))
	{
		Players.Add(ScoringPlayer);
	}
	
	Players.Sort([](const TObjectPtr<ADeathMatchPlayerState> A,
		const TObjectPtr<ADeathMatchPlayerState> B) {
		return A->GetScore() > B->GetScore();
	});

	TopScore = Players[0]->GetScore();
	
	TopScoringPlayers.Empty();
	TopScoringPlayers.Add(Players[0]);

	for (int i = 1; i < Players.Num(); i++)
	{
		if (Players[i]->GetScore() < TopScore) break;
		TopScoringPlayers.Add(Players[i]);
	}

	Notify();
	
}

FString ADeathMatchGameState::GetMatchResultStr(const ADeathMatchPlayerState* Player)
{
	if (TopScoringPlayers.Num() == 0)
	{
		return FString("There is no winner");
	}
	
	if (TopScoringPlayers.Num() > 1)
	{
		FString InfoTexString = FString("Players tied for the win: \n");
		for (const auto TiedPlayer : TopScoringPlayers)
		{
			InfoTexString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
		}
		return InfoTexString;
	}
	
	if (TopScoringPlayers[0] == Player)
	{
		return FString("You are the winner");
	}

	return FString::Printf(TEXT("Winner : \n%s"), *TopScoringPlayers[0]->GetPlayerName());
	
}

void ADeathMatchGameState::Attach(IObserver<TArray<TObjectPtr<ADeathMatchPlayerState>>>* Observer)
{
	Observers.insert(Observer);
	Observer->Update(Players);
}

void ADeathMatchGameState::Detach(IObserver<TArray<TObjectPtr<ADeathMatchPlayerState>>>* Observer)
{
	Observers.erase(Observer);
}

void ADeathMatchGameState::Notify()
{
	std::ranges::for_each(Observers, [&](IObserver<TArray<TObjectPtr<ADeathMatchPlayerState>>>* Observer) {
		Observer->Update(Players);
	});
}

