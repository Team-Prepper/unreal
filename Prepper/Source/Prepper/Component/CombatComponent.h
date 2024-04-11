#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Prepper/Enums/CombatState.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class APlayerCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void Reload();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	
protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	void ReloadEmptyWeapon();

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	void Fire();
	void FireProjectileWeapon();
	void FireHitScanWeapon();
	void FireShotgun();
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);
public:
	void FireButtonPressed(bool bPressed);
protected:
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	void TraceUnderCrosshair(FHitResult& TraceHitResult);

	void SetHUDCrosshair(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();
	int32 AmountToReload();
	
	bool bLocallyReload = false;
private:
	UPROPERTY()
	class APlayerCharacter* Character;
	UPROPERTY()
	class APrepperPlayerController* Controller;
	UPROPERTY()
	class APrepperHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class AWeapon* EquippedWeapon;
	
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming;

	bool bAimButtonPressed = false;
	

	UFUNCTION()
	void OnRep_Aiming();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FHUDPackage HUDPackage;
	FVector HitTarget;

	// Aiming FOV

	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomFOV = 30.f;

	float CurrentFOV;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	//Auto Fire
	FTimerHandle FireTimer;

	// State
	bool bCanFire = true;

	// chk Ammo
	bool CanFire();

	// carried ammo for cur equip weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;
	UPROPERTY(EditAnywhere)
	int32 StartingRevolverAmmo = 30;
	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo = 100;
	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 10;
	UPROPERTY(EditAnywhere)
	int32 StartingSniperRifleAmmo = 10;
	UPROPERTY(EditAnywhere)
	int32 StartingMiniGunAmmo = 0;
	void InitCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();
	void UpdateAmmoValues();
	void StartFireTimer();
	void FireTimerFinished();
	
};