// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DeathMatchPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API ADeathMatchPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void  GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// REPLICATION NOTIFY
	virtual void OnRep_Score() override;
	UFUNCTION()
	virtual void OnRep_Defeats();
	void AddToScore(float ScoreValue);
	void AddToDefeats(int32 DefeatsValue);
private:
	UPROPERTY()
	class APlayerCharacter* Character;
	UPROPERTY()
	class APrepperPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	
	
};
