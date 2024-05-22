// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Prepper/Weapon/Projectile/Projectile.h"

void AProjectileWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	Super::Fire(HitTargets);

	if (!HasAuthority()) return;
	FVector HitTarget = HitTargets.Top();
	
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetRangeWeaponMesh()->GetSocketByName(FName("Muzzle"));
	if (!MuzzleFlashSocket) return;
	
	FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetRangeWeaponMesh());
	FVector ToTarget = HitTarget - SocketTransform.GetLocation();
	FRotator TargetRotation = ToTarget.Rotation();
	
	if (!ProjectileClass || !InstigatorPawn) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = InstigatorPawn;
	UWorld* World = GetWorld();
	if (World)
	{
		World->SpawnActor<AProjectile>(
			ProjectileClass,
			SocketTransform.GetLocation(),
			TargetRotation,
			SpawnParams
			);
	}

	MakeNoise(1, PlayerOwnerCharacter, FVector::ZeroVector);
}