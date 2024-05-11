// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Prepper/Interfaces/Interactable.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* BASE */
	UInteractionComponent();
	friend class APlayerCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* SetInteractable */
	UFUNCTION()
	void SetItemInteractable(AActor* InteractableItem);
	UFUNCTION(Server, Reliable)
	void ServerSetItemInteractable(AActor* InteractableItem);

protected:
	UPROPERTY()
	class APlayerCharacter* Character;

	void TraceInteractionItem(FHitResult& TraceHitResult);

private:
	float TraceRange = 500.f;

	UPROPERTY()
	TScriptInterface<IInteractable> CurInteractableItem;
		
};
