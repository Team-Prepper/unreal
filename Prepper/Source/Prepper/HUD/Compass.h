// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Compass.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UCompass : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* CompassPoint;

	UPROPERTY()
	float CompassOffset;

	void SetDirection();
	void PossessPlayer();

	float GetImageWidth() const;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* PlayerCam;

	FTimerHandle SetDirectionTimerHandle;
	FTimerHandle PlayerPossessTimerHandle;
};
