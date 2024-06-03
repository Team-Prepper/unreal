// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeGameMode.h"

void AEscapeGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
	ABasePlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}
