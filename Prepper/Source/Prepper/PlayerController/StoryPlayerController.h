// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "StoryPlayerController.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PREPPER_API AStoryPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* Button1;
	
	virtual void PossessPlayerCharacter() override;
	virtual void SetInput(UEnhancedInputComponent* Input) override;
	
	void OpenInventoryPressed();

	void QuickSlot1Use();

	UFUNCTION(Server, Reliable)
	void ServerToggleInventory();
};
