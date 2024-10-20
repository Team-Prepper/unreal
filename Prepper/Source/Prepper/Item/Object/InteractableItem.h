#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prepper/Object/InteractableActor.h"
#include "InteractableItem.generated.h"

UCLASS()
class PREPPER_API AInteractableItem : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	AInteractableItem();
	virtual void Tick(float DeltaTime) override;
	virtual void Interaction(APlayerCharacter* Target) override;
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;
	
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FString ItemCode;

	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;
	
	void DestroyInteractionItem();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyInteractionItem();
};