// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Interactable.generated.h"

UCLASS()
class PREPPER_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void Interaction(APlayerCharacter* Target) PURE_VIRTUAL(AInteractable::Interaction, );
	virtual void ShowPickUpWidget(bool bShowWidget);
	
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
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickUpWidget;

	
};