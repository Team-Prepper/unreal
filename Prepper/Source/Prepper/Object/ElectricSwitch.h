// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "ElectricSwitch.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AElectricSwitch : public AInteractableObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString TargetAchievement;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TargetMonster;
	
public:
	virtual void Interaction(APlayerCharacter* Target) override;
	void CreateMonster();
};
