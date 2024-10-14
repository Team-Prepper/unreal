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

	UPROPERTY()
	class AInteractableBox* ControlledFallingActor;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnFallingActor();

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
