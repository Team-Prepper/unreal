#include "BaseCombatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Weapon/WeaponActor.h"

// Actor
UBaseCombatComponent::UBaseCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	EquippedWeapon = nullptr;
	
	bFireButtonPressed = false;
	bAiming = false;

	ReloadMontage = nullptr;
	AttackMontage = nullptr;

	Character = nullptr;
}

void UBaseCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBaseCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UBaseCombatComponent, bAiming);
	DOREPLIFETIME(UBaseCombatComponent, CombatState);
}


// Observer Pattern

void UBaseCombatComponent::Attach(IObserver<GaugeValue<int>>* Observer)
{
	AmmoObservers.insert(Observer);

	if (!EquippedWeapon)
	{
		Observer->Update(FGaugeInt(-1, -1));
		return;
	}
	Observer->Update(GetAmmoShow());
}

void UBaseCombatComponent::Detach(IObserver<GaugeValue<int>>* Observer)
{
	AmmoObservers.erase(Observer);
}

void UBaseCombatComponent::NotifyAmmo()
{
	const FGaugeInt Value = GetAmmoShow();
	std::ranges::for_each(AmmoObservers, [&](IObserver<GaugeValue<int>>* Observer) {
		Observer->Update(Value);
	});
}

void UBaseCombatComponent::Attach(IObserver<FString>* Observer)
{
	WeaponObservers.insert(Observer);

	if (!EquippedWeapon)
	{
		Observer->Update(FString(""));
		return;
	}
	Observer->Update(EquippedWeapon->GetCode());
}

void UBaseCombatComponent::Detach(IObserver<FString>* Observer)
{
	WeaponObservers.erase(Observer);
}

void UBaseCombatComponent::NotifyWeapon()
{
	FString Value = FString();
	
	if (EquippedWeapon)
	{
		Value = EquippedWeapon->GetCode();
	}
	
	std::ranges::for_each(WeaponObservers, [&](IObserver<FString>* Observer) {
		Observer->Update(Value);
	});
	
}
// Equipped Weapon
void UBaseCombatComponent::EquipWeapon(AWeaponActor* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (WeaponToEquip == nullptr) return;
	
	DropEquippedWeapon();
	
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	EquippedWeapon->SetWeaponHandler(this);
	
	ReloadEmptyWeapon();
	NotifyAmmo();
	NotifyWeapon();
}

void UBaseCombatComponent::EquipWeaponSet(AWeaponActor* WeaponToEquip)
{
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(Character);
}

void UBaseCombatComponent::DropEquippedWeapon()
{
	if (!EquippedWeapon) return;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	EquippedWeapon = nullptr;
}

void UBaseCombatComponent::OnRep_EquippedWeapon()
{
	NotifyWeapon();
	
	if (!EquippedWeapon) return;
	if (!Character) return;

	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UBaseCombatComponent::ActionEnd()
{
	ReloadEmptyWeapon();
	Fire();
}

// Fire
void UBaseCombatComponent::FireTrigger(bool bPressed)
{
	if (bFireButtonPressed == bPressed) return;
	
	bFireButtonPressed = bPressed;

	if (bFireButtonPressed)
	{
		Fire();
	}
}

void UBaseCombatComponent::Fire()
{
	if (!CanFire()) return;
	
	HitTargets = EquippedWeapon->GetTarget(HitTarget);
	
	LocalFireWeapon(HitTargets);
	ServerFireWeapon(HitTargets);

	CombatState = ECombatState::ECS_Fire;

	Character->GetWorldTimerManager().SetTimer(
		ActionTimer,
		this,
		&UBaseCombatComponent::FinishFire,
		EquippedWeapon->FireDelay
	);
}

bool UBaseCombatComponent::CanFire() const
{
	if (!EquippedWeapon) return false;
	if (!bFireButtonPressed) return false;
	if (CombatState != ECombatState::ECS_Unoccupied) return false;

	return EquippedWeapon->GetLeftAmmo() != 0;
}

void UBaseCombatComponent::LocalFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets) const
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	
	EquippedWeapon->Fire(TraceHitTargets);
	Character->PlayAnim(AttackMontage);
	
}

void UBaseCombatComponent::ServerFireWeapon_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets) const
{
	// 노이즈 발생
	EquippedWeapon->MakeNoise(1, Character, FVector::ZeroVector);
	MulticastFireWeapon(TraceHitTargets);
}

void UBaseCombatComponent::MulticastFireWeapon_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets) const
{
	if (Character && Character->IsLocallyControlled()) return;
	LocalFireWeapon(TraceHitTargets);
}

void UBaseCombatComponent::FinishFire()
{
	CombatState = ECombatState::ECS_Unoccupied;
	ActionEnd();
}

// Reload

void UBaseCombatComponent::Reload()
{
	if (CombatState == ECombatState::ECS_Reloading) return;
	if (!EquippedWeapon->CanReload()) return;
	
	ServerReload();
	HandleReload();
}

void UBaseCombatComponent::HandleReload() const
{
	if (!Character) return;

	Character->PlayAnim(ReloadMontage, EquippedWeapon->ReloadActionName);
}

void UBaseCombatComponent::ReloadEmptyWeapon()
{
	if (EquippedWeapon == nullptr || EquippedWeapon->GetLeftAmmo() == 0) return;
	
	Reload();
}
void UBaseCombatComponent::ServerReload_Implementation()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	if (!Character->IsLocallyControlled())
	{
		CombatState = ECombatState::ECS_Reloading;
		HandleReload();
	}
}

void UBaseCombatComponent::FinishReload()
{
	CombatState = ECombatState::ECS_Unoccupied;
	ActionEnd();
}

// Set Aiming
void UBaseCombatComponent::SetAiming(bool bIsAiming)
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	if (bAiming == bIsAiming) return;

	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	
	if (!Character->IsLocallyControlled()) return;
	
	bAimButtonPressed = bIsAiming;
}

void UBaseCombatComponent::OnRep_Aiming()
{
	if (Character && Character->IsLocallyControlled())
	{
		bAiming = bAimButtonPressed;
	}
}

void UBaseCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UBaseCombatComponent::SetCharacter(ABaseCharacter* Target)
{
	Character = Target;
}

void UBaseCombatComponent::TargetElim()
{
	FireTrigger(false);
	
	if(EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	}
}

// Combat State
void UBaseCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Reloading:
		if (Character && !Character->IsLocallyControlled())
		{
			HandleReload();
		}
		break;
	case ECombatState::ECS_Unoccupied:
		if (bFireButtonPressed)
		{
			Fire();
		}
		break;
	default:
		break;
	}
}