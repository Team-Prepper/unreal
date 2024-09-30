// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusWidget.h"

#include "Components/ProgressBar.h"
#include "Prepper/Character/Component/Status.h"

void UStatusWidget::Update(const Status& NewData)
{
	HungerBar->SetPercent(NewData.Hungry.GetRatio());
	ThirstBar->SetPercent(NewData.Thirsty.GetRatio());
	InfectionBar->SetPercent(NewData.Infected.GetRatio());
}