// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IDamageable
{
	GENERATED_BODY()
public:
	virtual void ReceiveDamage(
	AActor* DamagedActor,
	 float Damage,
	 const UDamageType* DamageType,
	 class AController* InstigatorController,
	 AActor* DamageCauser) PURE_VIRTUAL();
};
