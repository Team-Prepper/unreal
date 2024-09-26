// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryGameMode.h"

#include "Prepper/Mission/MissionChecker/DefaultMissionChecker.h"

void AStoryGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
                                      ABasePlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	
}

void AStoryGameMode::AddMission()
{
	
}
