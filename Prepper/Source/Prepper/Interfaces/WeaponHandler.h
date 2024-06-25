// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IWeaponHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Reload() PURE_VIRTUAL();
	virtual void Fire() PURE_VIRTUAL();
	virtual void EquipWeapon(class AWeaponActor* WeaponToEquip) PURE_VIRTUAL();
	
};
