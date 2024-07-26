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
	
// Actor
public:	
	UCombatComponent();
	friend class APlayerCharacter;
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
	
// Eqip Weapon
private:
	
	UPROPERTY(Replicated)
	class ARangeWeapon* EquippedRangeWeapon;

	UPROPERTY(Replicated)
	class AMeleeWeapon* EquippedMeleeWeapon;
	
	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void EquipPrimaryWeapon(AWeaponActor* WeaponToEquip);
	void EquipSecondaryWeapon(AWeaponActor* WeaponToEquip);
public:
	virtual void EquipWeapon(AWeaponActor* WeaponToEquip) override;
	
// Swap Weapon
private:
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;
	
	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();
	
	bool ShouldSwapWeapons();
// ActionReservation
private:
	TQueue<Action> ActionQueue;
public:
	virtual void ActionReservation(Action Act) override;
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
	
	bool CanFire();
	
	void LocalFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);
	
	void FireTimerFinished();
public:
	virtual void FireTrigger(bool IsTrigger) override;
	
	virtual void Fire() override;

// Reload
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;
	
	bool bLocallyReload = false;
	
	void HandleReload();
	void ReloadEmptyWeapon();

	UFUNCTION(Server, Reliable)
	void ServerReload();
	
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	
	int32 AmountToReload();
public:
	virtual void Reload() override;

// Set Aming
private:
	// Aiming FOV
	float DefaultFOV;
	
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming;
	
	UFUNCTION()
	void OnRep_Aiming();
	
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);
	
public:
	void SetAiming(bool bIsAiming);
	
public:
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeaponActor* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeaponActor* SecondaryWeapon;

// SetCrosshair
private:
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

// Targeting
protected:

	void TraceUnderCrosshair(FHitResult& TraceHitResult);
	void SetHUDCrosshair(float DeltaTime, FLinearColor& CrosshairColor);
	
	UPROPERTY()
	APlayerCharacter* Character;
	UPROPERTY()
	APrepperPlayerController* Controller;
	UPROPERTY()
	APrepperHUD* HUD;

	void DropEquippedWeapon();
	void UpdateCarriedAmmo();
	void SwapWeapons();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwapWeapon();
	
	FTimerHandle SwapDelayTimer;

	UFUNCTION()
	void SetWeaponType();

	bool bAimButtonPressed = false;

	FVector HitTarget;
	TArray<FVector_NetQuantize> HitTargets;
	
// Ammo
private:
	// carried ammo for cur equip weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;
public:
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);
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
	void InitCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();
	void UpdateAmmoValues();
};