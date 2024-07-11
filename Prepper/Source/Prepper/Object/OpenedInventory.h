#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenedInventory.generated.h"

class UMapInventory;
UCLASS()
class PREPPER_API AOpenedInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	AOpenedInventory();

	void SetTargetInventory(UMapInventory* Inventory);

protected:
	UFUNCTION()
	void InitInventory();

	UPROPERTY()
	UMapInventory* TargetInventory;

	UPROPERTY()
	class AInventoryInteractableItem* InventoryInteractableItem;
	
	UPROPERTY()
	TSubclassOf<class AInventoryInteractableItem> InventoryInteractableItemClass;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* BaseActor;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* InventoryMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCustomCameraComponent* InventoryCam;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = Camera)
	float DefaultCamArmLength;

	
};
