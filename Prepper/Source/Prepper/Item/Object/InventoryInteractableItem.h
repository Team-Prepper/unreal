// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem.h"
#include "InventoryInteractableItem.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API AInventoryInteractableItem : public AInteractableItem
{
	GENERATED_BODY()
public:
	AInventoryInteractableItem();
	virtual void Interaction(APlayerCharacter* Target) override;

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	UUserWidget* WidgetInstance;

	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* TouchedComponent);
	
	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
};
