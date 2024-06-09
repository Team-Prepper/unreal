// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerAbility.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IPlayerAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OpenCraftingTable() PURE_VIRTUAL();
	
	virtual void AddItem(FString ItemCode) PURE_VIRTUAL();
	virtual void UseQuickSlotItem(int Idx) PURE_VIRTUAL();
	virtual void EquipWeapon(class AWeaponActor* Weapon) PURE_VIRTUAL();
	virtual void AttachActorAtSocket(const FName& SocketName, AActor* TargetActor) PURE_VIRTUAL();

	virtual void Heal(int Amount) PURE_VIRTUAL();
	virtual void Eat(int Amount) PURE_VIRTUAL();
	virtual void Drink(int Amount) PURE_VIRTUAL();
};
