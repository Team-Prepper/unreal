// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "StoryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AStoryPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
protected:
	virtual void PossessPlayerCharacter() override;
};
