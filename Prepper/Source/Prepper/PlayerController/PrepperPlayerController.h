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
	virtual void Tick(float DeltaTime) override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Value);
	void SetHUDCarriedAmmo(int32 Value);
	void SetHUDMatchCountDown(float CountDownTime);
	virtual void OnPossess(APawn* InPawn) override;
protected:
	virtual void BeginPlay() override;

	void SetHUDTime();
private:
	UPROPERTY()
	class APrepperHUD* PrepperHUD;

	/* 플레이 시간 카운터 */
	UPROPERTY(EditAnywhere)
	float MatchTime = 120.f;
	uint32 CountdownInt = 0;
	
};
