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
	/*
	UFUNCTION()
	void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);
	*/
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UStaticMeshComponent* MeleeWeaponMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "WeaponTracer")
	class UBoxComponent* WeaponTracer;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* TracerStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TracerEnd;

	UFUNCTION()
	void DamageTarget(const FHitResult& HitTarget);
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBoxComponent() const { return WeaponTracer; }
	
};
