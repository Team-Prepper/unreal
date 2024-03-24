// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "UObject/Interface.h"
#include "Controllable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControllable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IControllable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Move(const FInputActionValue& Value) PURE_VIRTUAL(IControllable::Move, );
	virtual void Look(const FInputActionValue& Value) PURE_VIRTUAL(IControllable::Look, );
	
	virtual void ShiftPressed() PURE_VIRTUAL(IControllable::ShiftPressed, );
	virtual void ShiftReleased() PURE_VIRTUAL(IControllable::ShiftReleased, );
	
	virtual void EPressed() PURE_VIRTUAL(IControllable::EPressed, );
	virtual void RPressed() PURE_VIRTUAL(IControllable::RPressed, );
	
	virtual void ControlPressed() PURE_VIRTUAL(IControllable::ControlPressed, );

	virtual void MouseLeftPressed() PURE_VIRTUAL(IControllable::MouseLeftPressed, );
	virtual void MouseLeftReleased() PURE_VIRTUAL(IControllable::MouseLeftReleased, );
	virtual void MouseRightPressed() PURE_VIRTUAL(IControllable::MouseRightPressed, );
	virtual void MouseRightReleased() PURE_VIRTUAL(IControllable::MouseRightReleased, );
	
};
