// Fill out your copyright notice in the Description page of Project Settings.


#include "SensitivityChangeEffect.h"

#include "Prepper/Character/PlayerCharacter.h"

void USensitivityChangeEffect::PlayerAimingStart(const TObjectPtr<APlayerCharacter> PlayerCharacter)
{
	TargetPlayer = PlayerCharacter;
	TargetPlayer->SetSensitivity(Effect);
}

void USensitivityChangeEffect::PlayerAimingEnd()
{
	TargetPlayer->SetSensitivity(1);
}
