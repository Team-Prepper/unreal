// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DeathMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API ADeathMatchGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PlayerEliminated(class APlayerCharacter* ElimmedCharacter, class APrepperPlayerController* VictimController, APrepperPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
};
