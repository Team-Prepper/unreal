// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperGameMode.h"
#include "Prepper/Character/BaseCharacter.h"

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
	ABasePlayerController* AttackerController)
{
	PlayerEliminated(ElimmedCharacter);
}

void APrepperGameMode::SaveGame(const APlayerCharacter* TargetPlayerCharacter)
{
	
}

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}
