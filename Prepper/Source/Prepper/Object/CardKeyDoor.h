// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "CardKeyDoor.generated.h"

UCLASS()
class PREPPER_API ACardKeyDoor : public AInteractableActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Door Properties")
	FString TargetItemCode = FString("CardKey");

	UPROPERTY(EditAnywhere, Category="Door Properties")
	bool IsClosed = true;
public:
	// Sets default values for this actor's properties
	ACardKeyDoor();

	virtual void Interaction(APlayerCharacter* Target) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();
	UFUNCTION(BlueprintImplementableEvent)
	void CloseDoor();
	
};
