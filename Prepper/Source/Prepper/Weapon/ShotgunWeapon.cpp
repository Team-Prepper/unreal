// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Prepper/Character/PlayerCharacter.h"

void AShotgunWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleSocket = GetRangeWeaponMesh()->GetSocketByName("Muzzle");
	if (MuzzleSocket)
	{
		const FTransform SocketTransform = MuzzleSocket->GetSocketTransform(GetRangeWeaponMesh());
		const FVector Start = SocketTransform.GetLocation();

		// hit Character - number of hit
		TMap<ABaseCharacter*, uint32> HitMap;
		for(FVector_NetQuantize HitTarget : HitTargets)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ABaseCharacter* DamagedCharacter = Cast<ABaseCharacter>(FireHit.GetActor());
			if (DamagedCharacter)
			{
				if (HitMap.Contains(DamagedCharacter))
				{
					HitMap[DamagedCharacter]++;
				}
				else
				{
					HitMap.Emplace(DamagedCharacter, 1);
				}
				if(ImpactParticles)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						ImpactParticles,
						FireHit.ImpactPoint,
						FireHit.ImpactNormal.Rotation()
					);
				}
				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(
						this,
						HitSound,
						FireHit.ImpactPoint,
						.5f,
						FMath::FRandRange(-.5f, .5f)
					);
				}
			}
		}
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && HasAuthority() && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					HitPair.Key,
					Damage * HitPair.Value,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
	
}

TArray<FVector_NetQuantize> AShotgunWeapon::GetTarget(FVector& HitTarget)
{
	TArray<FVector_NetQuantize> HitTargets;
	ShotgunTraceEndWithScatter(HitTarget, HitTargets);
	return HitTargets;
}

void AShotgunWeapon::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets)
{
	const USkeletalMeshSocket* MuzzleSocket = GetRangeWeaponMesh()->GetSocketByName("Muzzle");
	if (MuzzleSocket == nullptr) return;

	const FTransform SocketTransform = MuzzleSocket->GetSocketTransform(GetRangeWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	for (uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector EndLoc = SphereCenter + RandVec;
		FVector ToEndLoc = EndLoc - TraceStart;
		ToEndLoc = TraceStart + ToEndLoc * TRACE_LEN / ToEndLoc.Size();

		HitTargets.Add(ToEndLoc);
	}
}
