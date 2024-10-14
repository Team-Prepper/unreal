// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBoxSpawner.h"

#include "Prepper/Item/Object/InteractableBox.h"

// Sets default values
AWeaponBoxSpawner::AWeaponBoxSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	FallingSpeed = 100.0f;
}

void AWeaponBoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBoxSpawner::SpawnFallingActor()
{
	if (FallingActorClass)
	{
		// 스폰할 위치 설정
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += SpawnHeight;

		FRotator SpawnRotation = FRotator::ZeroRotator;

		ControlledFallingActor = GetWorld()->SpawnActor<AInteractableBox>(FallingActorClass, SpawnLocation, SpawnRotation);
		ControlledFallingActor->BoxMesh->SetSimulatePhysics(true);
		ControlledFallingActor->BoxMesh->SetEnableGravity(true);
	}
}

void AWeaponBoxSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnFallingActor();
}

