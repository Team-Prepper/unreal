// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class PREPPER_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interaction(APlayerCharacter* Target) PURE_VIRTUAL(IIInteractable::Interaction, );
	virtual void ShowPickUpWidget(bool bShowWidget) PURE_VIRTUAL(IIInteractable::ShowPickUpWidget, );
};
