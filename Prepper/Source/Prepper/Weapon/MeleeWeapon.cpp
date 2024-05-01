#include "MeleeWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"


void AMeleeWeapon::Fire(const FVector& HitTarget)
{
	FindActorsWithinRadius();
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
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);
	
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
		DrawDebugSphere(World, Hit.GetActor()->GetActorLocation(), 16.f, 12, FColor::Purple, false, 5.0f);
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
	APlayerCharacter* DamagedCharacter = Cast<APlayerCharacter>(HitTarget.GetActor());
	if(DamagedCharacter == Cast<APlayerCharacter>(GetOwner())) return;
	
	if (DamagedCharacter && HasAuthority() && InstigatorController)
	{
		UGameplayStatics::ApplyDamage(
			DamagedCharacter,
			Damage,
			InstigatorController,
			this,
			UDamageType::StaticClass()
		);
	}
	//DrawDebugSphere(GetWorld(), HitTarget.ImpactPoint, 16.f, 12, FColor::Purple, false, 5.0f);
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
