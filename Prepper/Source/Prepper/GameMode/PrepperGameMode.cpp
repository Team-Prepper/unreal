// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperGameMode.h"
#include "Prepper/Character/BaseCharacter.h"

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, APrepperPlayerController* VictimController,
	APrepperPlayerController* AttackerController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}
