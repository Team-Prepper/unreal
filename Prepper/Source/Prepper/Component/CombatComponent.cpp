#include "CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/Weapon/MeleeWeapon.h"
#include "Prepper/Weapon/WeaponActor.h"
#include "Prepper/Weapon/RangeWeapon/RangeWeapon.h"
#include "Prepper/_Base/Util/GaugeInt.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	DOREPLIFETIME(UCombatComponent, EquippedRangeWeapon);
	DOREPLIFETIME(UCombatComponent, EquippedMeleeWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UCombatComponent, CombatState);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Character) return;
	
	UE_LOG(LogTemp, Warning, TEXT("CombatComponentReady"));
	Character->SetPlayerMovementState(EPlayerMovementState::EPMS_Idle);

	if (Character->GetFollowCamera())
	{
		DefaultFOV = Character->GetFollowCamera()->FieldOfView;
	}
	if (Character->HasAuthority())
	{
		InitCarriedAmmo();
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Character) return;
	if (!Character->IsLocallyControlled()) return;
	
	// 총구의 방향을 내 화면의 방향과 일치 시키기 위해서 틱에서 처리 
	FHitResult HitResult;
	TraceUnderCrosshair(HitResult);

	if (HitResult.GetActor() &&
		HitResult.GetActor()->Implements<UInteractWithCrosshairInterface>())
	{
		HUDPackage.CrosshairColor = FLinearColor::Red;
	}
	else
	{
		HUDPackage.CrosshairColor = FLinearColor::White;
	}
	if (HitResult.bBlockingHit)
	{
		HitTarget = HitResult.ImpactPoint;
	}
	else
	{
		HitTarget = HitResult.Location;
	}

	SetHUDCrosshair(DeltaTime);
}

void UCombatComponent::SetHUDCrosshair(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr) return;
	if (!EquippedWeapon) return;

	Controller = Controller == nullptr ? Cast<APrepperPlayerController>(Character->Controller) : Controller;
	if (!Controller) return;
	
	HUD = HUD == nullptr ? Cast<APrepperHUD>(Controller->GetHUD()) : HUD;
	if (!HUD) return;
	
	EquippedWeapon->GetCrosshair(DeltaTime, bAiming,
								HUDPackage.CrosshairCenter,
								HUDPackage.CrosshairLeft,
								HUDPackage.CrosshairRight,
								HUDPackage.CrosshairTop,
								HUDPackage.CrosshairBottom,
								HUDPackage.CrosshairSpread);

	HUD->SetHUDPackage(HUDPackage);
}

// TODO
// Fire Start
void UCombatComponent::FireButtonPressed(bool bPressed)
{
	if (bFireButtonPressed == bPressed) return;
	
	bFireButtonPressed = bPressed;

	if (bFireButtonPressed)
	{
		Fire();
	}
}
// TODO
bool UCombatComponent::CanFire()
{
	if (!EquippedWeapon) return false;
	if (CombatState != ECombatState::ECS_Unoccupied) return false;

	if (EquippedMeleeWeapon) return true;
	return !EquippedRangeWeapon->IsAmmoEmpty();
}

void UCombatComponent::Fire()
{
	if (!CanFire()) return;
	
	CrosshairShootingFactor = .75f;
	HitTargets = EquippedWeapon->GetTarget(HitTarget);
	LocalFireWeapon(HitTargets);
	ServerFireWeapon(HitTargets);

	CombatState = ECombatState::ECS_Fire;

	Notify();
	Character->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay
	);
}

void UCombatComponent::LocalFireWeapon(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	
	EquippedWeapon->Fire(TraceHitTargets);	
	
	if (EquippedRangeWeapon)
	{
		Character->PlayAnim(FireWeaponMontage, bAiming ? FName("FireAim") : FName("FireHip"));
		
	}
	else
	{
		Character->PlayAnim(MeleeWeaponMontage, EquippedMeleeWeapon->GetWeaponType() == EWeaponType::EWT_MeleeWeaponBlunt ? FName("Attack1") : FName("Attack2"));
	}
	
}

void UCombatComponent::ServerFireWeapon_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	// 노이즈 발생
	EquippedWeapon->MakeNoise(1, Character, FVector::ZeroVector);
	MulticastFireWeapon(TraceHitTargets);
}

void UCombatComponent::MulticastFireWeapon_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (Character && Character->IsLocallyControlled()) return;
	LocalFireWeapon(TraceHitTargets);
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;

	CombatState = ECombatState::ECS_Unoccupied;
	EquippedWeapon->FireEnd(bFireButtonPressed);
}

void UCombatComponent::ReloadEmptyWeapon()
{
	if (EquippedRangeWeapon && EquippedRangeWeapon->IsAmmoEmpty())
	{
		Reload();
	}
}


void UCombatComponent::OnRep_CarriedAmmo()
{
	Notify();
}

void UCombatComponent::InitCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Revolver, StartingRevolverAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SMG, StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, StartingSniperRifleAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_MiniGun, StartingMiniGunAmmo);
}

void UCombatComponent::PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	if (CarriedAmmoMap.Contains(WeaponType))
	{
		CarriedAmmoMap[WeaponType] = FMath::Clamp(CarriedAmmoMap[WeaponType] + AmmoAmount, 0, MaxCarriedAmmo);
		UpdateCarriedAmmo();
	}
	if (EquippedRangeWeapon && EquippedRangeWeapon->IsAmmoEmpty() && EquippedWeapon->GetWeaponType() == WeaponType)
	{
		Reload();
	}
}

EWeaponType UCombatComponent::SetWeaponType()
{
	if (EquippedWeapon)
	{
		EquippedRangeWeapon = Cast<ARangeWeapon>(EquippedWeapon);
		EquippedMeleeWeapon = Cast<AMeleeWeapon>(EquippedWeapon);
		return EquippedWeapon->GetWeaponType();
	}
	return EWeaponType::EWT_MAX;
}


void UCombatComponent::OnRep_Aiming()
{
	if (Character && Character->IsLocallyControlled())
	{
		bAiming = bAimButtonPressed;
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	if (bAiming == bIsAiming) return;

	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	
	if (!Character->IsLocallyControlled()) return;
	
	EPlayerMovementState NewState = bIsAiming ? EPlayerMovementState::EPMS_Aim : EPlayerMovementState::EPMS_Idle;
	Character->SetPlayerMovementState(NewState);
	bAimButtonPressed = bIsAiming;
	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		Character->ShowSniperScopeWidget(bIsAiming);
	}

	if (bIsAiming)
	{
		Character->GetFollowCamera()
			->InterpFOV(EquippedRangeWeapon->GetZoomedFOV(),
			EquippedRangeWeapon->GetZoomedFOV());
		return;
	}

	Character->GetFollowCamera()
			->InterpFOV(DefaultFOV,
			EquippedRangeWeapon->GetZoomedFOV());
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		EPlayerMovementState NewState = bIsAiming ? EPlayerMovementState::EPMS_Aim : EPlayerMovementState::EPMS_Idle;
		Character->SetPlayerMovementState(NewState);
	}
}


void UCombatComponent::EquipWeapon(AWeaponActor* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	WeaponToEquip->SetActorEnableCollision(false);

	if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
	{
		EquipSecondaryWeapon(WeaponToEquip);
	}
	else
	{
		EquipPrimaryWeapon(WeaponToEquip);
	}

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (!EquippedWeapon || !Character) return;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	Character->AttachActorAtSocket(EquippedWeapon->AttachSocketName(), EquippedWeapon);

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
	if (SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Holstered);
	}
}

void UCombatComponent::EquipPrimaryWeapon(AWeaponActor* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	DropEquippedWeapon();
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	SetWeaponType();
	UpdateCarriedAmmo();
	ReloadEmptyWeapon();
}

void UCombatComponent::EquipSecondaryWeapon(AWeaponActor* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetOwner(Character);
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Holstered);
}

void UCombatComponent::DropEquippedWeapon()
{
	if (!EquippedWeapon) return;
	
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	EquippedMeleeWeapon = nullptr;
	EquippedRangeWeapon = nullptr;
}

void UCombatComponent::SwapWeapons()
{
	if (CombatState != ECombatState::ECS_Unoccupied || Character == nullptr || !Character->HasAuthority()) return;
	MulticastSwapWeapon();
}

void UCombatComponent::MulticastSwapWeapon_Implementation()
{
	CombatState = ECombatState::ECS_SwappingWeapons;

	FinishSwapAttachWeapons();
	GetWorld()->GetTimerManager().SetTimer(SwapDelayTimer, this, &UCombatComponent::FinishSwap, 1.5f, false);

	Character->PlayAnim(SwapMontage);
	
}

void UCombatComponent::FinishSwap()
{
	if (Character)
	{
		CombatState = ECombatState::ECS_Unoccupied;
	}
}

void UCombatComponent::FinishSwapAttachWeapons()
{
	if (Character == nullptr) return;
	AWeaponActor* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	SetWeaponType();
	UpdateCarriedAmmo();
	ReloadEmptyWeapon();

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Holstered);
	
	Notify();
}

void UCombatComponent::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr) return;
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
	else
	{
		CarriedAmmo = -1;
	}
	
	Notify();
}

void UCombatComponent::Reload()
{
	if (CarriedAmmo <= 0) return;
	if (CombatState == ECombatState::ECS_Reloading) return;
	if (!EquippedWeapon->CanReload()) return;
	
	ServerReload();
	HandleReload();
}

void UCombatComponent::ServerReload_Implementation()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	if (!Character->IsLocallyControlled())
	{
		CombatState = ECombatState::ECS_Reloading;
		HandleReload();
	}
}

void UCombatComponent::FinishReloading()
{
	if (Character == nullptr) return;
	CombatState = ECombatState::ECS_Unoccupied;
	if (Character->HasAuthority())
	{
		UpdateAmmoValues();
	}
	if (bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::Attach(IObserver<GaugeValue<int>>* Observer)
{
	Observers.insert(Observer);

	if (!EquippedWeapon)
	{
		Observer->Update(FGaugeInt(-1, -1));
		return;
	}
	
	int WeaponAmmo = -1;
	
	if (EquippedWeapon)
	{
		WeaponAmmo = EquippedWeapon->GetLeftAmmo();
	}
	Observer->Update(FGaugeInt(WeaponAmmo, CarriedAmmo));
}

void UCombatComponent::Detach(IObserver<GaugeValue<int>>* Observer)
{
	Observers.erase(Observer);
}

void UCombatComponent::Notify()
{
	int WeaponAmmo = -1;
	
	if (EquippedWeapon)
	{
		WeaponAmmo = EquippedWeapon->GetLeftAmmo();
	}

	FGaugeInt Value(WeaponAmmo, CarriedAmmo);
	std::ranges::for_each(Observers, [&](IObserver<GaugeValue<int>>* Observer) {
		Observer->Update(Value);
	});
}

void UCombatComponent::UpdateAmmoValues()
{
	if (Character == nullptr || EquippedRangeWeapon == nullptr) return;
	int32 ReloadAmount = AmountToReload();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}
	EquippedRangeWeapon->AddAmmo(ReloadAmount);
	Notify();
}

int32 UCombatComponent::AmountToReload()
{
	if (EquippedRangeWeapon == nullptr) return 0;
	int32 RoomInMag = EquippedRangeWeapon->GetMagCapacity() - EquippedRangeWeapon->GetAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least);
	}
	return 0;
}

void UCombatComponent::OnRep_CombatState()
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
	}
}

void UCombatComponent::HandleReload()
{
	if (!Character) return;

	Character->PlayAnim(ReloadMontage, EquippedWeapon->ReloadActionName);
}


void UCombatComponent::TraceUnderCrosshair(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	const FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	const bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (!bScreenToWorld) return;
	
	FVector Start = CrosshairWorldPosition;

	if (Character)
	{
		const float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
	}
	
	const FVector End = Start + CrosshairWorldDirection * TRACE_LEN;
	const ECollisionChannel CollisionChannel = ECC_Visibility;

	const bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
		TraceHitResult, Start, End, CollisionChannel);
	
	if (!bHitSomething)
	{
		// TraceHitResult의 Location에 End 좌표 설정
		TraceHitResult.Location = End;
	}
}

bool UCombatComponent::ShouldSwapWeapons()
{
	return (EquippedWeapon != nullptr && SecondaryWeapon != nullptr);
}