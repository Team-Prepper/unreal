// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemCombinationData.generated.h"

USTRUCT(BlueprintType)
struct PREPPER_API FItemCombinationData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp")
	FString InputItemCode1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp")
	FString InputItemCode2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp")
	FString OutputItemCode;
};

