// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prepper/Weapon/WeaponActor.h"
#include "WeaponData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PREPPER_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	FString WeaponCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	TSubclassOf<AWeaponActor> WeaponClass;
};
