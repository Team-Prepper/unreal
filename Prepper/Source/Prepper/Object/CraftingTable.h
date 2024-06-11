#pragma once

#include "CoreMinimal.h"
#include "Prepper/Object/InteractableActor.h"
#include "CraftingTable.generated.h"


UCLASS()
class PREPPER_API ACraftingTable : public AInteractableActor
{
	GENERATED_BODY()

public:
	ACraftingTable();
	virtual void Interaction(APlayerCharacter* Target) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* TableMesh;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInteraction(APlayerCharacter* Target);

	void ShowCraftingWidget(APlayerCharacter* Target);
	
};