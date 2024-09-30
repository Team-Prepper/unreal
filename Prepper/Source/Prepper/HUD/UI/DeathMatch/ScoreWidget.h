// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PREPPER_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category=DeathMatch)
	UTextBlock* ScoreValue;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category=DeathMatch)
	UTextBlock* DefeatsValue;
public:
	void SetScore(float Score) const;
	void SetDefeat(int Defeat) const;
};
