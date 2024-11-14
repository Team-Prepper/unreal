// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperGameMode.h"
#include "Prepper/Character/BaseCharacter.h"
#include "Prepper/PlayerController/BasePlayerController.h"

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
                                        ABasePlayerController* AttackerController)
{
	PlayerEliminated(ElimmedCharacter);

	if (VictimController) VictimController->Elim();
}

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}
