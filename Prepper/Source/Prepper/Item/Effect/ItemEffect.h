// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prepper/Interfaces/PlayerAbility.h"
#include "UObject/Interface.h"
#include "ItemEffect.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemEffect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IItemEffect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Action(IPlayerAbility* Target) PURE_VIRTUAL( IItemEffect::Action, );
};
