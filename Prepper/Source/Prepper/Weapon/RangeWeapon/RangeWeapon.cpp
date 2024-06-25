#include "RangeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"

void ARangeWeapon::SetHUDAmmo()
{
	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;
	if(PlayerOwnerCharacter)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
		if(PlayerOwnerController)
		{
			PlayerOwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

void ARangeWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if(Owner)
	{
		SetHUDAmmo();
	}
}

void ARangeWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	FVector HitTarget = HitTargets.Top();
	TargetDistance = FVector::Distance(HitTarget, WeaponMesh->GetComponentLocation());
	
	if(FireAnimation)
	{
		GetRangeWeaponMesh()->PlayAnimation(FireAnimation, false);
	}
	
	SpendRound();
}

void ARangeWeapon::FireEnd(bool Trigger)
{
	if (WeaponHandler == nullptr) return;
	if (Trigger && bAutomatic)
	{
		WeaponHandler->Fire();
	}
	if (bAutoReload)
	{
		WeaponHandler->Reload();
	}
}

bool ARangeWeapon::IsAmmoEmpty()
{
	return Ammo <= 0;
}

bool ARangeWeapon::IsAmmoFull()
{
	return Ammo == MagCapacity;
}

USkeletalMeshComponent* ARangeWeapon::GetRangeWeaponMesh()
{
	if(RangeWeaponMesh == nullptr)
	{
		RangeWeaponMesh = Cast<USkeletalMeshComponent>(WeaponMesh);
	}
	return RangeWeaponMesh;
}

void ARangeWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo -1, 0, MagCapacity);
	SetHUDAmmo();
	if(HasAuthority())
	{
		ClientUpdateAmmo(Ammo);
	}
	else if (PlayerOwnerCharacter && PlayerOwnerCharacter->IsLocallyControlled())
	{
		Sequence++;
	}
}

void ARangeWeapon::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{
	if (HasAuthority()) return;
 
	Ammo = ServerAmmo;
	--Sequence;
	Ammo -= Sequence;
	SetHUDAmmo();
}

void ARangeWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	SetHUDAmmo();
	ClientAddAmmo(AmmoToAdd);
}

void ARangeWeapon::ClientAddAmmo_Implementation(int32 AmmoToAdd)
{
	if (HasAuthority()) return;
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	SetHUDAmmo();
}

TArray<FVector_NetQuantize> ARangeWeapon::GetTarget(FVector& HitTarget)
{
	TArray<FVector_NetQuantize> HitTargets;
	if(bUseScatter)
	{
		HitTarget = TraceEndWithScatter(HitTarget);
	}
	
	HitTargets.Add(HitTarget);
	return HitTargets;
}

void ARangeWeapon::GetCrosshair(float DeltaTime, bool bIsAiming, UTexture2D*& Center, UTexture2D*& Left,
	UTexture2D*& Right, UTexture2D*& Top, UTexture2D*& Bottom, float &Spread)
{
	Center = CrosshairCenter;
	Left   = CrosshairLeft;
	Right  = CrosshairRight;
	Top    = CrosshairTop;
	Bottom = CrosshairBottom;
	Spread = 0.5f;

	if (bUseScatter || PlayerOwnerCharacter == nullptr) {
		return;
	}
	
	FVector2D WalkSpeedRange(0.f, PlayerOwnerCharacter->GetCharacterMovement()->MaxWalkSpeed);
	FVector2D VelocityMultiplierRange(0.f, 1.f);
	FVector Velocity = PlayerOwnerCharacter->GetVelocity();
	Velocity.Z = 0.f;

	const float CrosshairVelocityFactor =
		FMath::GetMappedRangeValueClamped(
			WalkSpeedRange,
			VelocityMultiplierRange,
			Velocity.Size());

	if (PlayerOwnerCharacter->GetCharacterMovement()->IsFalling())
	{
		CurCrosshairInAirFactor =
			FMath::FInterpTo(
				CurCrosshairInAirFactor,
				MaxCrosshairInAirFactor,
				DeltaTime, 2.25f);
	}
	else
	{
		CurCrosshairInAirFactor =
			FMath::FInterpTo(
				CurCrosshairInAirFactor,
				0,DeltaTime, 2.25f);
	}

	if (bIsAiming)
	{
		CurCrosshairAimFactor =
			FMath::FInterpTo(CurCrosshairAimFactor, MaxCrosshairAimFactor, DeltaTime, 30.f);
	}
	else
	{
		CurCrosshairAimFactor =
			FMath::FInterpTo(CurCrosshairAimFactor, 0.f, DeltaTime, 30.f);
	}

	CurCrosshairShootingFactor =
		FMath::FInterpTo(CurCrosshairShootingFactor, 0.f, DeltaTime, 40.f);

	Spread += CrosshairVelocityFactor +
		CurCrosshairInAirFactor -
		CurCrosshairAimFactor +
		CurCrosshairShootingFactor;
	
}

FVector ARangeWeapon::TraceEndWithScatter(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleSocket = GetRangeWeaponMesh()->GetSocketByName("Muzzle");
	if (MuzzleSocket == nullptr) return FVector();
	
	const FTransform SocketTransform = MuzzleSocket->GetSocketTransform(GetRangeWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();
	
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	const FVector EndLoc = SphereCenter + RandVec;
	const FVector ToEndLoc = EndLoc - TraceStart;

	return FVector(TraceStart + ToEndLoc * TRACE_LEN / ToEndLoc.Size());
}
