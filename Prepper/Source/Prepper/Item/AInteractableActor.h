// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Interfaces/Interactable.h"
#include "AInteractableActor.generated.h"

UCLASS()
class PREPPER_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	virtual void ShowPickUpWidget(bool bShowWidget) override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
		);

	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	class USphereComponent* AreaSphere;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickUpWidget;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int32 CustomDepthColor;
	
};