#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

class UNiagaraSystem;

UCLASS()
class PREPPER_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AMeleeWeapon();
	virtual void Fire(const FVector& HitTarget) override;

	void FindActorsWithinRadius();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UStaticMeshComponent* MeleeWeaponMesh;


	UFUNCTION()
	void DamageTarget(const FHitResult& HitTarget);
	
};
