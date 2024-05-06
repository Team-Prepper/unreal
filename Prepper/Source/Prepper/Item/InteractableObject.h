// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AInteractableActor.h"
#include "InteractableObject.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AInteractableObject : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	virtual void Interaction(APlayerCharacter *Target) override;
	
	/** Called when the brake lights are turned on or off */
	UFUNCTION(BlueprintImplementableEvent)
	void InteractAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	USkeletalMeshComponent* ItemMesh;


};
