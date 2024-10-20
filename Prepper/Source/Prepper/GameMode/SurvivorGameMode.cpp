// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorGameMode.h"

void ASurvivorGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
                                         ABasePlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	
}

void ASurvivorGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PlayTime += DeltaSeconds;
}

void ASurvivorGameMode::AddMission()
{
	
}

float ASurvivorGameMode::GetPlayTime() const
{
	return PlayTime;
}

void ASurvivorGameMode::SetPlayTime(const float Time)
{
	PlayTime = Time;
}
