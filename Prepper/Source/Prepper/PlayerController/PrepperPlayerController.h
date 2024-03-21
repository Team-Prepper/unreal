// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrepperPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API APrepperPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Value);
	void SetHUDCarriedAmmo(int32 Value);
	virtual void OnPossess(APawn* InPawn) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class APrepperHUD* PrepperHUD;
	
};
