// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GaugeValue.h"
#include "UObject/NoExportTypes.h"
#include "GaugeFloat.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FGaugeFloat
{
	GENERATED_USTRUCT_BODY()
private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	UPROPERTY(EditAnywhere, Category = Combat)
	float CurValue;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float MaxValue;
	
public:
	FGaugeFloat();
	virtual ~FGaugeFloat();
	virtual void AddValue(float& Value);
	virtual void SubValue(float& Value);
	virtual float GetCurValue();
	virtual float GetMaxValue();
	virtual float GetRatio();
	
};

