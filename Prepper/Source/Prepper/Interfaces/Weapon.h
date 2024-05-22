// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Prepper/Weapon/WeaponTypes.h"
#include "Weapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PREPPER_API IWeapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void Fire(const TArray<FVector_NetQuantize>& HitTargets)
	PURE_VIRTUAL();

	virtual void SetHUDAmmo()
	PURE_VIRTUAL();

	virtual void GetCrosshair(UTexture2D* &Center, UTexture2D* &Left, UTexture2D* &Right, UTexture2D* &Top, UTexture2D* &Bottom)
	PURE_VIRTUAL();
	
	virtual void SetWeaponState(EWeaponState State)
	PURE_VIRTUAL();

	virtual void SetOwnerActor(AActor* Owner)
	PURE_VIRTUAL();

	virtual float GetFireDelay()
	PURE_VIRTUAL(IWeapon::GetFireDelay, return 0.f; );

	virtual FName AttachSocketName()
	PURE_VIRTUAL(IWeapon::AttachSocketName(), return FName("RightHandSocket");)
	
	virtual FName GetReloadActionName()
	PURE_VIRTUAL(IWeapon::GetReloadActionName(), return FName("RightHandSocket");)
	
	virtual TArray<FVector_NetQuantize> GetTarget(FVector& HitTarget)
	PURE_VIRTUAL(IWeapon::GetTarget(FVector& HitTarget) , TArray<FVector_NetQuantize> arr; return arr; );

	virtual EWeaponType GetWeaponType()
	PURE_VIRTUAL(IWeapon::GetWeaponType , return EWeaponType::EWT_AssaultRifle; );
};
