// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Interactable.generated.h"

UCLASS(Abstract)
class PREPPER_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void Interaction(APlayerCharacter* Target) PURE_VIRTUAL(AInteractable::Interaction, );
	virtual void ShowPickUpWidget(bool bShowWidget) PURE_VIRTUAL(AInteractable::ShowPickUpWidget, );;
};