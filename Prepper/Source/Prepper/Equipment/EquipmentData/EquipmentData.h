// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.generated.h"

class AEquipment;
/**
 * 
 */
USTRUCT(BlueprintType)
struct PREPPER_API FEquipmentData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	FString EquipmentCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	TSubclassOf<AEquipment> EquipmentClass;
	
	FEquipmentData() : EquipmentCode(TEXT("DefaultString1")), EquipmentClass(nullptr)
	{
	}
};
