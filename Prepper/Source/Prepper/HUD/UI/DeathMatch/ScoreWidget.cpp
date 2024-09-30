// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

#include "Components/TextBlock.h"

void UScoreWidget::SetScore(float Score) const
{
	FString ScoreText = FString::Printf(TEXT("%d"),FMath::FloorToInt(Score));
	ScoreValue->SetText(FText::FromString(ScoreText));
}

void UScoreWidget::SetDefeat(int Defeat) const
{
	FString DefeatsText = FString::Printf(TEXT("%d"), Defeat);
	DefeatsValue->SetText(FText::FromString(DefeatsText));
}
