#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Prepper/Enums/CombatState.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/Interfaces/WeaponHandler.h"
#include "Prepper/Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UCombatComponent : public UActorComponent, public IWeaponHandler, public ISubject<GaugeValue<int>>
{
	GENERATED_BODY()
public:
	friend class APlayerCharacter;
// Actor
public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;
	
// Observer Pattern
private:
	std::pmr::set<IObserver<GaugeValue<int>>*> Observers;

public:
	virtual void Attach(IObserver<GaugeValue<int>>* Observer) override;
	virtual void Detach(IObserver<GaugeValue<int>>* Observer) override;
	virtual void Notify() override;
	
	// Equip Weapon
private:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeaponActor* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeaponActor* SecondaryWeapon;
	
	UPROPERTY(Replicated)
	class ARangeWeapon* EquippedRangeWeapon;

	UPROPERTY(Replicated)
	class AMeleeWeapon* EquippedMeleeWeapon;
	
public:
	virtual void EquipWeapon(AWeaponActor* WeaponToEquip) override;

private:
	void EquipPrimaryWeapon(AWeaponActor* WeaponToEquip);
	void EquipSecondaryWeapon(AWeaponActor* WeaponToEquip);
	void DropEquippedWeapon();
	
	UFUNCTION()
	void SetWeaponType();
	
	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION()
	void OnRep_SecondaryWeapon();
	
// Swap Weapon
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;
	
	FTimerHandle SwapDelayTimer;
public:
	void SwapWeapons();
	
private:
	
	bool ShouldSwapWeapons();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwapWeapon();
	
	UFUNCTION(BlueprintCallable)
	void FinishSwap();
	
	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();
	
// Action Reservation
private:
	TQueue<EAction> ActionQueue;
public:
	virtual void ActionReservation(EAction Act) override;
	void ActionDequeue();
// Fire
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* MeleeWeaponMontage;

	//Auto Fire
	FTimerHandle FireTimer;
	
	bool bFireButtonPressed;

public:
	virtual void FireTrigger(bool IsTrigger) override;
	
	virtual void Fire() override;

private:
	
	bool CanFire();
	
	void LocalFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);
	
	void FireTimerFinished();

// Reload
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;
	
	bool bLocallyReload = false;
	
public:
	virtual void Reload() override;
	
private:
	void HandleReload();
	void ReloadEmptyWeapon();
	int32 AmountToReload();

	UFUNCTION(Server, Reliable)
	void ServerReload();
	
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

// Set Aiming
private:
	// Aiming FOV
	float DefaultFOV;
	
	bool bAimButtonPressed = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming;
	
public:
	void SetAiming(bool bIsAiming);

private:
	UFUNCTION()
	void OnRep_Aiming();
	
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

// Set Crosshair
private:
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	void SetHUDCrosshair(float DeltaTime, const FLinearColor& CrosshairColor);
	
// Targeting
protected:
	
	UPROPERTY()
	APlayerCharacter* Character;
	UPROPERTY()
	APrepperPlayerController* Controller;
	UPROPERTY()
	APrepperHUD* HUD;
	
	FVector HitTarget;
	TArray<FVector_NetQuantize> HitTargets;
	
	void TraceUnderCrosshair(FHitResult& TraceHitResult);
	
// Ammo
private:
	// carried ammo for cur equip weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;
	
protected:
	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

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
	
public:
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

private:
	void InitCarriedAmmo();
	void UpdateCarriedAmmo();
	void UpdateAmmoValues();
	
	UFUNCTION()
	void OnRep_CarriedAmmo();

// Combat State
private:
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;
	UFUNCTION()
	void OnRep_CombatState();
};