// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameMode.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"

void ADeathMatchGameMode::PlayerEliminated(APlayerCharacter* ElimmedCharacter,
                                           APrepperPlayerController* VictimController, APrepperPlayerController* AttackerController)
{
	ADeathMatchPlayerState* AttackPlayerState = AttackerController ? Cast<ADeathMatchPlayerState>(AttackerController->PlayerState) : nullptr;
	ADeathMatchPlayerState* VictimPlayerState = VictimController ? Cast<ADeathMatchPlayerState>(VictimController -> PlayerState) : nullptr;

	if(AttackPlayerState &&  AttackPlayerState != VictimPlayerState)
	{
		AttackPlayerState->AddToScore(1.0f);
	}
	if(VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void ADeathMatchGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
		
	}
}
