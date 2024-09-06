// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AimingEffect.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAimingEffect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IAimingEffect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetDefaultFieldOfView() PURE_VIRTUAL( IAimingEffect::GetDefaultFieldOfView, return 0; );
	virtual void ShowSniperScopeWidget(bool bIsAiming) PURE_VIRTUAL();
	virtual void InterpFOV(float GoalFOVSet, float InterpSpeedSet) PURE_VIRTUAL();
};
