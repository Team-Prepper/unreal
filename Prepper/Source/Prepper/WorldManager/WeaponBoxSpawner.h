// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBoxSpawner.generated.h"

UCLASS()
class PREPPER_API AWeaponBoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBoxSpawner();

	UPROPERTY(Replicated)
	class AInteractableBox* ControlledFallingActor;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DropActor();

	UFUNCTION(BlueprintCallable,Server,Reliable, Category="Spawning")
	void SpawnFallingActor_Server();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	TSubclassOf<AInteractableBox> FallingActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	float SpawnHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Falling")
	float FallingSpeed;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
