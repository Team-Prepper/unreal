// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterComponent.generated.h"

class ABaseCharacter;

// This class does not need to be modified.
UINTERFACE()
class UCharacterComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API ICharacterComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetCharacter(ABaseCharacter* Target) PURE_VIRTUAL();
	virtual void TargetElim() PURE_VIRTUAL();
};
