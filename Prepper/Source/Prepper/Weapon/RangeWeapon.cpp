#include "RangeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
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