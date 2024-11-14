// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricSwitch.h"

#include "Prepper/GameMode/SurvivorGameMode.h"

void AElectricSwitch::Interaction(APlayerCharacter* Target)
{
	Super::Interaction(Target);
	ASurvivorGameMode* GM = GetWorld()->GetAuthGameMode<ASurvivorGameMode>();
	if (GM == nullptr) return;

	GM->GetAchievement();
	CreateMonster();
	
}

void AElectricSwitch::CreateMonster()
{
	GetWorld()->SpawnActor(TargetMonster);
}
