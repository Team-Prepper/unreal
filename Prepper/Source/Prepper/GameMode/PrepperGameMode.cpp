// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepperGameMode.h"
#include "Prepper/Character/BaseCharacter.h"

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
	ABasePlayerController* AttackerController)
{
	PlayerEliminated(ElimmedCharacter);
}

void APrepperGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
		RemoveCharacterAfterDelay(ElimmedCharacter, DelayTime);
	}
}

void APrepperGameMode::RemoveCharacterAfterDelay(ACharacter* Character, float Delay)
{
	if (Character)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
        
		TimerDelegate.BindUObject(this, &APrepperGameMode::RemoveCharacter, Character);
        
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay, false);
	}
}

void APrepperGameMode::RemoveCharacter(ACharacter* Character)
{
	if (Character)
	{
		Character->Destroy();
	}
}
