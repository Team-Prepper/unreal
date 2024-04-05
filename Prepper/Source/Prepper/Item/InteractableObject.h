// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AInteractable.h"
#include "InteractableObject.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AInteractableObject : public AInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction(APlayerCharacter *Target) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	USkeletalMeshComponent* ItemMesh;


};
