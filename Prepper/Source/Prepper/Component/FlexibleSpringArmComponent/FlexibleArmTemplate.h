// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FlexibleArmTemplate.generated.h"

/**
 * 
 */
USTRUCT()
struct PREPPER_API FFlexibleArmTemplate
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Length;
	UPROPERTY(EditAnywhere)
	FVector Location;
};
