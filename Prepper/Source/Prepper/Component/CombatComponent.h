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
	
	void EquipWeapon(class AWeaponActor* WeaponToEquip);
	void Reload();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* MeleeWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;
	
protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	void ReloadEmptyWeapon();

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);
	
	void Fire();
	void LocalFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();
	
public:
	void FireButtonPressed(bool bPressed);
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class AWeaponActor* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeaponActor* SecondaryWeapon;
protected:

	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets);

	void TraceUnderCrosshair(FHitResult& TraceHitResult);

	void SetHUDCrosshair(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();
	int32 AmountToReload();
	
	bool bLocallyReload = false;
	
	UPROPERTY()
	class APlayerCharacter* Character;
	UPROPERTY()
	class APrepperPlayerController* Controller;
	UPROPERTY()
	class APrepperHUD* HUD;

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void EquipPrimaryWeapon(AWeaponActor* WeaponToEquip);
	void EquipSecondaryWeapon(AWeaponActor* WeaponToEquip);

	void DropEquippedWeapon();
	void UpdateCarriedAmmo();
	void SwapWeapons();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwapWeapon();
	
	FTimerHandle SwapDelayTimer;
	
	UPROPERTY(Replicated)
	class ARangeWeapon* EquippedRangeWeapon;

	UPROPERTY(Replicated)
	class AMeleeWeapon* EquippedMeleeWeapon;

	UFUNCTION()
	EWeaponType SetWeaponType();
	
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming;

	bool bAimButtonPressed = false;

	UFUNCTION()
	void OnRep_Aiming();

	bool bFireButtonPressed;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FHUDPackage HUDPackage;
	FVector HitTarget;
	TArray<FVector_NetQuantize> HitTargets;
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


	// chk Ammo
	bool CanFire();

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
	void StartFireTimer();
	void FireTimerFinished();
	bool ShouldSwapWeapons();
};