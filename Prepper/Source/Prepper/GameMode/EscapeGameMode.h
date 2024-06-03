// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrepperGameMode.h"
#include "EscapeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AEscapeGameMode : public APrepperGameMode
{
	GENERATED_BODY()

	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter,
									class ABasePlayerController* VictimController,
									ABasePlayerController* AttackerController);
};
