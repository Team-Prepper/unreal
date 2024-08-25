// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerComponent.generated.h"

class APlayerCharacter;

// This class does not need to be modified.
UINTERFACE()
class UPlayerComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IPlayerComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetPlayer(APlayerCharacter* Target) PURE_VIRTUAL();
	virtual void TargetElim() PURE_VIRTUAL();
};
