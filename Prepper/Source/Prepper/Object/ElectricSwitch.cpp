// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricSwitch.h"

#include "Prepper/Character/BombCharacter.h"
#include "Prepper/GameMode/SurvivorGameMode.h"

void AElectricSwitch::Interaction(APlayerCharacter* Target)
{
	Super::Interaction(Target);
	ASurvivorGameMode* GM = GetWorld()->GetAuthGameMode<ASurvivorGameMode>();
	if (GM == nullptr) return;

	if (GM->IsAchieved(TargetAchievement)) return;
	// 막는걸로?
	
	CreateMonster();
	
}

void AElectricSwitch::CreateMonster()
{
	GetWorld()->SpawnActor<ABombCharacter>(TargetMonster)->SetElectricSwitch(this);
}

void AElectricSwitch::DefenderRemove()
{
	ASurvivorGameMode* GM = GetWorld()->GetAuthGameMode<ASurvivorGameMode>();
	
	if (GM == nullptr) return;
	GM->AddAchievement(TargetAchievement, true);
}
