// Fill out your copyright notice in the Description page of Project Settings.


#include "GaugeFloat.h"

FGaugeFloat::FGaugeFloat()
{
	CurValue = 0;
	MaxValue = 0;
}

FGaugeFloat::~FGaugeFloat()
{
}

void FGaugeFloat::AddValue(float& Value)
{
	CurValue += Value;
	if (CurValue > MaxValue) CurValue = MaxValue;
}

void FGaugeFloat::SubValue(float& Value)
{
	CurValue -= Value;
	if (CurValue < 0) CurValue = 0;
}

float FGaugeFloat::GetCurValue()
{
	return CurValue;
}

float FGaugeFloat::GetMaxValue()
{
	return MaxValue;
}

float FGaugeFloat::GetRatio()
{
	return CurValue / MaxValue;
}