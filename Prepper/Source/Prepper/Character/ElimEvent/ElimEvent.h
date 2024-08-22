﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ElimEvent.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UElimEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IElimEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void StartElim() PURE_VIRTUAL();
	virtual void SetTarget(ACharacter* Target) PURE_VIRTUAL();
	
};
