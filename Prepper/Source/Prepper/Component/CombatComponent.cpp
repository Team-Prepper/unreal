#include "CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/Weapon/MeleeWeapon.h"
#include "Prepper/Weapon/ShotgunWeapon.h"
#include "Prepper/Weapon/Weapon.h"
#include "Sound/SoundCue.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 400.f;
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

	if(Character)
	{
		UE_LOG(LogTemp,Warning,TEXT("CombatComponentReady"));
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if(Character->GetFollowCamera())
		{
			DefaultFOV = Character->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
		if(Character->HasAuthority())
		{
			InitCarriedAmmo();
		}
		
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Character && Character->IsLocallyControlled())
	{
		// 총구의 방향을 내 화면의 방향과 일치 시키기 위해서 틱에서 처리 
		FHitResult HitResult;
        TraceUnderCrosshair(HitResult);
		if(HitResult.bBlockingHit)
		{
			HitTarget = HitResult.ImpactPoint;
		}
		else
		{
			HitTarget = HitResult.Location;
		}

		SetHUDCrosshair(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

void UCombatComponent::SetHUDCrosshair(float DeltaTime)
{
	if( Character == nullptr || Character->Controller == nullptr) return;

	Controller = Controller == nullptr ? Cast<APrepperPlayerController>(Character->Controller) : Controller;
	if(Controller)
	{
		HUD = HUD == nullptr ? Cast<APrepperHUD>(Controller->GetHUD()) : HUD;
		if(HUD)
		{
			if(EquippedRangeWeapon)
			{
				HUDPackage.CrosshairCenter = EquippedRangeWeapon->CrosshairCenter;
				HUDPackage.CrosshairLeft   = EquippedRangeWeapon->CrosshairLeft;
				HUDPackage.CrosshairRight  = EquippedRangeWeapon->CrosshairRight;
				HUDPackage.CrosshairTop    = EquippedRangeWeapon->CrosshairTop;
				HUDPackage.CrosshairBottom = EquippedRangeWeapon->CrosshairBottom;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairLeft   = nullptr;
				HUDPackage.CrosshairRight  = nullptr;
				HUDPackage.CrosshairTop    = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
			}

			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			if(bAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			HUDPackage.CrosshairSpread =
					0.5f +
					CrosshairVelocityFactor +
					CrosshairInAirFactor -
					CrosshairAimFactor +
					CrosshairShootingFactor;
			
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}


void UCombatComponent::InterpFOV(float DeltaTime)
{
	if(EquippedRangeWeapon == nullptr) return;

	if(bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedRangeWeapon->GetZoomedFOV(), DeltaTime, EquippedRangeWeapon->GetZoomedInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	if(Character && Character->GetFollowCamera())
	{
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

// TODO
// Fire Start
void UCombatComponent::FireButtonPressed(bool bPressed)
{
	if(EquippedRangeWeapon && EquippedRangeWeapon->bAutomatic == false)
	{
		if(bFireButtonPressed == bPressed) return;
	}
	bFireButtonPressed = bPressed;
	
	if(bFireButtonPressed)
    {
        Fire();
    }
}

// TODO
bool UCombatComponent::CanFire()
{
	if(EquippedMeleeWeapon) return bCanFire;
	
	if(EquippedRangeWeapon == nullptr) return false;
	
	if(bLocallyReload) return false;
	return !EquippedRangeWeapon->IsAmmoEmpty() &&
			bCanFire &&
			CombatState == ECombatState::ECS_Unoccupied;
}

// TODO
void UCombatComponent::Fire()
{
	if (!CanFire()) return;
	
	bCanFire = false;
	if (EquippedRangeWeapon)
	{
		CrosshairShootingFactor = .75f;
		switch (EquippedRangeWeapon->FireType)
		{
		case EFireType::EFT_Projectile:
			FireProjectileWeapon();
			break;
		case EFireType::EFT_Shotgun:
			FireShotgun();
			break;
		default:
			break;
		}
	}
	else
	{
		FireMeleeWeapon();
	}
	StartFireTimer();
}

void UCombatComponent::FireProjectileWeapon()
{
	if(EquippedRangeWeapon)
	{
		HitTarget = EquippedRangeWeapon->bUseScatter ? EquippedRangeWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
			
		LocalFire(HitTarget);
		ServerFire(HitTarget);
	}
	
}

void UCombatComponent::FireHitScanWeapon()
{
	if (EquippedRangeWeapon)
	{
		HitTarget = EquippedRangeWeapon->bUseScatter ? EquippedRangeWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		LocalFire(HitTarget);
		ServerFire(HitTarget);
	}
}

void UCombatComponent::FireShotgun()
{
	AShotgunWeapon* Shotgun = Cast<AShotgunWeapon>(EquippedWeapon);
	if (Shotgun)
	{
		TArray<FVector_NetQuantize> HitTargets;
		Shotgun->ShotgunTraceEndWithScatter(HitTarget, HitTargets);
		ShotgunLocalFire(HitTargets);
		ServerShotgunFire(HitTargets);
	}
}

void UCombatComponent::FireMeleeWeapon()
{
	if(EquippedMeleeWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("MELEE WEAPON ATTACK"));
		LocalFire(HitTarget);
		ServerFire(HitTarget);
	}
}

void UCombatComponent::LocalFire(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr) return;
	if(EquippedWeapon)
	{
		if (Character && (CombatState == ECombatState::ECS_Unoccupied))
        {
        	Character->PlayFireMontage(bAiming);
        	EquippedWeapon->Fire(TraceHitTarget);
        }
	}
}

void UCombatComponent::ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	AShotgunWeapon* ShotgunWeapon = Cast<AShotgunWeapon>(EquippedWeapon);
	if(EquippedWeapon == nullptr || Character == nullptr) return;
	if (CombatState == ECombatState::ECS_Unoccupied)
	{
		Character->PlayFireMontage(bAiming);
		ShotgunWeapon->FireShotgun(TraceHitTargets);
	}
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFire(TraceHitTarget);
}

void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (Character && Character->IsLocallyControlled()) return;
	LocalFire(TraceHitTarget);
}

void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	MulticastShotgunFire(TraceHitTargets);
}

void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (Character && Character->IsLocallyControlled()) return;
	ShotgunLocalFire(TraceHitTargets);
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay
	);
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;
	if(!EquippedRangeWeapon) return;
	if (bFireButtonPressed && EquippedRangeWeapon->bAutomatic)
	{
		Fire();
	}
	if(EquippedRangeWeapon -> GetAutoReload())
	{
		ReloadEmptyWeapon();
	}
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
	Controller = Controller == nullptr ? Cast<APrepperPlayerController>(Character->Controller) : Controller;
	if(Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
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
	if(EquippedWeapon)
	{
		EquippedRangeWeapon = Cast<ARangeWeapon>(EquippedWeapon);
		if(EquippedRangeWeapon)
		{
			return EquippedRangeWeapon->GetWeaponType();
		}
		EquippedMeleeWeapon = Cast<AMeleeWeapon>(EquippedWeapon);
		if(EquippedMeleeWeapon)
		{
			return EquippedMeleeWeapon->GetWeaponType();
		}
	}
	return EWeaponType::EWT_MAX;
}


void UCombatComponent::OnRep_Aiming()
{
	if(Character && Character->IsLocallyControlled())
	{
		bAiming = bAimButtonPressed;
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	if(Character == nullptr || EquippedWeapon == nullptr) return;
	if(bAiming == bIsAiming) return;
	
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
	if(Character->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		Character->ShowSniperScopeWidget(bIsAiming);
	}
	if(Character->IsLocallyControlled())
	{
		bAimButtonPressed = bIsAiming;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}


void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

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
	if (EquippedWeapon && Character)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		const USkeletalMeshSocket* HandSocket =
			Character->GetMesh()->GetSocketByName( EquippedWeapon->GetWeaponType() == EWeaponType::EWT_MeleeWeapon ? FName("MeleeWeaponSocket") : FName("RightHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
		}
		
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;

		if (EquippedWeapon->EquipSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				EquippedWeapon->EquipSound,
				Character->GetActorLocation()
			);
		}
	}
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
	if (SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Holstered);
		AttachActorToBackpack(SecondaryWeapon);
		PlayEquipWeaponSound(EquippedWeapon);
	}
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	DropEquippedWeapon();
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	SetWeaponType();
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	PlayEquipWeaponSound(WeaponToEquip);
	ReloadEmptyWeapon();
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Holstered);
	AttachActorToBackpack(WeaponToEquip);
	PlayEquipWeaponSound(WeaponToEquip);
	SecondaryWeapon->SetOwner(Character);
	SecondaryWeapon->EnableCustomDepth(false);
}

void UCombatComponent::DropEquippedWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* HandSocket;
	HandSocket = Character->GetMesh()->GetSocketByName(
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_MeleeWeapon ? FName("MeleeWeaponSocket") : FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, Character->GetMesh());
	}
}

void UCombatComponent::AttachActorToBackpack(AActor* ActorToAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;

	AWeapon* AttachWeapon = Cast<AWeapon>(ActorToAttach);
	if(AttachWeapon)
	{
		if(AttachWeapon->GetWeaponType() == EWeaponType::EWT_RocketLauncher)
		{
			const USkeletalMeshSocket* RocketLauncherSocket = Character->GetMesh()->GetSocketByName(FName("RocketLauncherSocket"));
			if (RocketLauncherSocket)
			{
				RocketLauncherSocket->AttachActor(ActorToAttach, Character->GetMesh());
			}
		}
		else
		{
			const USkeletalMeshSocket* HolsteredWeaponSocket = Character->GetMesh()->GetSocketByName(FName("HolsteredWeaponSocket"));
			if (HolsteredWeaponSocket)
			{
				HolsteredWeaponSocket->AttachActor(ActorToAttach, Character->GetMesh());
			}
		}
	}
	
}

void UCombatComponent::SwapWeapons()
{
	if (CombatState != ECombatState::ECS_Unoccupied || Character == nullptr || !Character->HasAuthority()) return;

	Character->PlaySwapMontage();
	CombatState = ECombatState::ECS_SwappingWeapons;
	Character->bFinishedSwapping = false;
	if (SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(false);

	
	FinishSwapAttachWeapons();
	FinishSwap();
}

void UCombatComponent::FinishSwap()
{
	if (Character)
	{
		CombatState = ECombatState::ECS_Unoccupied;
		Character->bFinishedSwapping = true;
	}
}

void UCombatComponent::FinishSwapAttachWeapons()
{
	PlayEquipWeaponSound(SecondaryWeapon);

	if (Character == nullptr) return;
	AWeapon* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	SetWeaponType();
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmo();
	ReloadEmptyWeapon();

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_Holstered);
	AttachActorToBackpack(SecondaryWeapon);
}

void UCombatComponent::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr) return;
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	Controller = Controller == nullptr ? Cast<APrepperPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::PlayEquipWeaponSound(AWeapon* WeaponToEquip)
{
	if (Character && WeaponToEquip && WeaponToEquip->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			WeaponToEquip->EquipSound,
			Character->GetActorLocation()
		);
	}
}

void UCombatComponent::Reload()
{
	if (CarriedAmmo > 0 &&
		CombatState != ECombatState::ECS_Reloading &&
		EquippedRangeWeapon &&
		!EquippedRangeWeapon -> IsAmmoFull() &&
		!bLocallyReload)
	{
		ServerReload();
		HandleReload();
		bLocallyReload = true;
	}
}

void UCombatComponent::ServerReload_Implementation()
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	CombatState =  ECombatState::ECS_Reloading;
	if(!Character->IsLocallyControlled())
	{
		HandleReload();
	}
}

void UCombatComponent::FinishReloading()
{
	if (Character == nullptr) return;
	bLocallyReload = false;
	CombatState = ECombatState::ECS_Unoccupied;
	if (Character->HasAuthority())
	{
		UpdateAmmoValues();
	}
	if(bFireButtonPressed)
	{
		Fire();
	}
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
	Controller = Controller == nullptr ? Cast<APrepperPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		Controller->SetHUDCarriedAmmo(CarriedAmmo);
	}
	EquippedRangeWeapon->AddAmmo(ReloadAmount);	
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
			if(Character && !Character->IsLocallyControlled())
			{
				HandleReload();
			}
			break;
		case ECombatState::ECS_Unoccupied:
			if(bFireButtonPressed)
			{
				Fire();
			}
			break;
	}
}

void UCombatComponent::HandleReload()
{
	if(Character){
		Character->PlayReloadMontage(EquippedWeapon->ReloadActionName);
	}
}

void UCombatComponent::TraceUnderCrosshair(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		}

		FVector End = Start + CrosshairWorldDirection * TRACE_LEN;

		 bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!bHitSomething)
		{
			// TraceHitResult의 Location에 End 좌표 설정
			TraceHitResult.Location = End;
		}
		
		if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairInterface>())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;
		}
	}
}

bool UCombatComponent::ShouldSwapWeapons()
{
	return (EquippedWeapon != nullptr && SecondaryWeapon != nullptr);
}
