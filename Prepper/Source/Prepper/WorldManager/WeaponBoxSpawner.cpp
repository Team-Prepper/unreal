#include "WeaponBoxSpawner.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Item/Object/InteractableBox.h"

// Sets default values
AWeaponBoxSpawner::AWeaponBoxSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	FallingSpeed = 100.0f;
	bReplicates = true;
}

void AWeaponBoxSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBoxSpawner, ControlledFallingActor);
}

void AWeaponBoxSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SpawnFallingActor_Server();
	}
}

void AWeaponBoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBoxSpawner::SpawnFallingActor_Server_Implementation()
{
	if (FallingActorClass)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += SpawnHeight;

		FRotator SpawnRotation = FRotator::ZeroRotator;

		ControlledFallingActor = GetWorld()->SpawnActor<AInteractableBox>(FallingActorClass, SpawnLocation, SpawnRotation);
		if (ControlledFallingActor)
		{
			ControlledFallingActor->SetReplicates(true);
			Multicast_DropActor();
		}
	}
}

void AWeaponBoxSpawner::Multicast_DropActor_Implementation()
{
	if (ControlledFallingActor && ControlledFallingActor->BoxMesh)
	{
		ControlledFallingActor->BoxMesh->SetSimulatePhysics(true);
		ControlledFallingActor->BoxMesh->SetEnableGravity(true);

		// 클라이언트에서도 물리 시뮬레이션이 활성화되도록 클라이언트 함수 호출
		ControlledFallingActor->BoxMesh->SetIsReplicated(true);
	}
}
