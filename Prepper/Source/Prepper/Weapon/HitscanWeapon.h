// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Weapon.h"
#include "HitscanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AHitscanWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

    UPROPERTY(EditAnywhere)
    UNiagaraSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;
	
private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;
};
