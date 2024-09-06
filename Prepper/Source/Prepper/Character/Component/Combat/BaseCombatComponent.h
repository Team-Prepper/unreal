#pragma once

#include "CoreMinimal.h"
#include "../PlayerComponent.h"
#include "Components/ActorComponent.h"
#include "Prepper/Character/BaseCharacter.h"
#include "Prepper/Enums/CombatState.h"
#include "Prepper/Interfaces/WeaponHandler.h"
#include "BaseCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UBaseCombatComponent : public UActorComponent,
									public IWeaponHandler, public IPlayerComponent
{
	GENERATED_BODY()
// Actor
public:	
	UBaseCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Equip Weapon
protected:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeaponActor* EquippedWeapon;
	
public:
	virtual void EquipWeapon(AWeaponActor* WeaponToEquip) override;
	void EquipWeaponSet(AWeaponActor* WeaponToEquip);

protected:
	virtual void DropEquippedWeapon();
	UFUNCTION()
	void OnRep_EquippedWeapon() const;
	
protected:
	virtual void ActionEnd() {};
// Fire
protected:
	//Auto Fire
	FTimerHandle ActionTimer;
	bool bFireButtonPressed;

public:
	virtual void FireTrigger(bool IsTrigger) override;
	
	virtual void Fire() override;

protected:
	
	bool CanFire() const;
	
	virtual void LocalFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets) const;

	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets) const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets) const;
	
	virtual void FinishFire();

// Reload
protected:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;
	
	bool bLocallyReload = false;
	
public:
	virtual void Reload() override;
	void HandleReload() const;
	
protected:
	void ReloadEmptyWeapon();

	UFUNCTION(Server, Reliable)
	void ServerReload();
	
	UFUNCTION(BlueprintCallable)
	virtual void FinishReload();

// Set Aiming
protected:
	bool bAimButtonPressed = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming;
	
public:
	virtual void SetAiming(bool bIsAiming);

private:
	UFUNCTION()
	void OnRep_Aiming();
	
	UFUNCTION(Server, Reliable)
	virtual void ServerSetAiming(bool bIsAiming);

// Targeting
protected:
	
	UPROPERTY()
	ABaseCharacter* Character;
	
	FVector HitTarget;
	TArray<FVector_NetQuantize> HitTargets;

public:
	virtual void SetPlayer(APlayerCharacter* Target) override;
	virtual void TargetElim() override;
	
// Combat State
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;
	UFUNCTION()
	void OnRep_CombatState();
};