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
	FVector StartLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * AttackReach;
	FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(AttackRange);
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);

	GetWorld()->SweepMultiByObjectType(
		HitResults,
		StartLocation,
		StartLocation, // End 위치는 시작 위치와 같음 (구체 검사)
		FQuat::Identity, // 회전 없음
		ObjectQueryParams,
		SphereCollisionShape
	);
	
	for (const FHitResult& Hit : HitResults)
	{
		DamageTarget(Hit);
		DrawDebugSphere(World, Hit.GetActor()->GetActorLocation(), 16.f, 12, FColor::Purple, false, 5.0f);
	}
	DrawDebugSphere(World, StartLocation, AttackRange, 24, FColor::Emerald, false, 5.0f);
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
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(HitTarget.GetActor());
	if (PlayerCharacter && HasAuthority() && InstigatorController)
	{
		UGameplayStatics::ApplyDamage(
			PlayerCharacter,
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
