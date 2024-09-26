// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PrepperGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API APrepperGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter);
	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter,
									class ABasePlayerController* VictimController,
									ABasePlayerController* AttackerController);

};
