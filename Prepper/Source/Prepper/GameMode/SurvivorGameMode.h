// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrepperGameMode.h"
#include "Prepper/Mission/Mission.h"
#include "UObject/Object.h"
#include "SurvivorGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API ASurvivorGameMode : public APrepperGameMode
{
	GENERATED_BODY()
private:
	int EnemyKillCount;
public:
	virtual void PlayerEliminated(ABaseCharacter* ElimmedCharacter,
									ABasePlayerController* VictimController,
									ABasePlayerController* AttackerController) override;
	void AddMission();
	int GetEnemyKillCount() const { return EnemyKillCount; }
	
};
