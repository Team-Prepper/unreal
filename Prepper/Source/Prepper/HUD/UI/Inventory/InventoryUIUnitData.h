// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryUIUnitData.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UInventoryUIUnitData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	APlayerCharacter* TargetPlayer;
	FString ItemName;
	FString ItemCode;
	unsigned int ItemCount;
};
