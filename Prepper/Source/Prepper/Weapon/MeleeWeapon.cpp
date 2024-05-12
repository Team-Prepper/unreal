#include "MeleeWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"


void AMeleeWeapon::Fire(const TArray<FVector_NetQuantize>& HitTargets)
{
	FindActorsWithinRadius();
}

TArray<FVector_NetQuantize> AMeleeWeapon::GetTarget(FVector& HitTarget)
{
	TArray<FVector_NetQuantize> HitTargets;
	FVector MeleeTypeVector = FVector(0.f, 0.f, 0.f);
	switch (WeaponType)
	{
	case EWeaponType::EWT_MeleeWeaponBlunt:
		MeleeTypeVector = FVector(1.f, 1.f, 1.f);
		break;
	case EWeaponType::EWT_MeleeWeaponSword:
		MeleeTypeVector = FVector(2.f, 2.f, 2.f);
		break;
		default:
			break;
	}
	
	HitTargets.Add(MeleeTypeVector);
	return HitTargets;
}

void AMeleeWeapon::FindActorsWithinRadius()
{
	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World!"));
		return;
	}
	
	TArray<FHitResult> HitResults;
	FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(AttackRange);
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_Pawn);
	
	GetWorld()->SweepMultiByObjectType(
        	HitResults,
        	Owner->GetActorLocation(),
        	Owner->GetActorLocation() + Owner->GetActorForwardVector() * AttackReach,
        	FQuat::Identity,
        	ObjectQueryParams,
        	SphereCollisionShape
        );
	
	
	for (const FHitResult& Hit : HitResults)
	{
		DamageTarget(Hit);
	}
	
}

void AMeleeWeapon::SetHUDAmmo()
{
	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;
	if(PlayerOwnerCharacter)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
		if(PlayerOwnerController)
		{
			PlayerOwnerController->SetHUDWeaponAmmo(-1);
		}
	}
}

void AMeleeWeapon::DamageTarget(const FHitResult& HitTarget)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();
	IDamageable* DamagedTarget = Cast<IDamageable>(HitTarget.GetActor());
	if(DamagedTarget == Cast<IDamageable>(GetOwner())) return;
	
	if (DamagedTarget && HasAuthority() && InstigatorController)
	{
		UGameplayStatics::ApplyDamage(
			HitTarget.GetActor(),
			Damage,
			InstigatorController,
			this,
			UDamageType::StaticClass()
		);
	}
	if(ImpactParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactParticles,
			HitTarget.GetActor()->GetActorLocation(),
			HitTarget.ImpactNormal.Rotation()
		);
	}
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
		this,
		HitSound,
		HitTarget.ImpactPoint);
	}
}

