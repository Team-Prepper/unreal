// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PrepperGameMode.h"
#include "DeathMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API ADeathMatchGameMode : public APrepperGameMode
{
	GENERATED_BODY()
	
public:
	ADeathMatchGameMode();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PlayerEliminated(class APlayerCharacter* ElimmedCharacter,
									class APrepperPlayerController* VictimController,
									APrepperPlayerController* AttackerController) override;
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:
	float CountdownTime = 0.f;
};
