// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoomInEffect.h"

#include "Prepper/Character/PlayerCharacter.h"

void UZoomInEffect::PlayerAimingStart(const TObjectPtr<APlayerCharacter> PlayerCharacter)
{
	TargetPlayer = PlayerCharacter;
	UE_LOG(LogTemp,Warning, TEXT("%f"), DefaultFOV);
	TargetPlayer->GetFollowCamera()->InterpFOV(GoalFOV, ZoomSpeed);
}

void UZoomInEffect::PlayerAimingEnd()
{
	UE_LOG(LogTemp,Warning, TEXT("%f"), DefaultFOV);
	TargetPlayer->GetFollowCamera()->InterpFOV(TargetPlayer->GetFollowCamera()->DefaultFOV, ZoomSpeed);
	
}
