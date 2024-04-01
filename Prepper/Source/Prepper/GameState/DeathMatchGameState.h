// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DeathMatchGameState.generated.h"


UCLASS()
class PREPPER_API ADeathMatchGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
	void UpdateTopScore(class ADeathMatchPlayerState* ScoringPlayer);
	
	UPROPERTY(Replicated)
	TArray<ADeathMatchPlayerState* >TopScoringPlayers;
	
private:
	float TopScore = 0.f;
	
};
