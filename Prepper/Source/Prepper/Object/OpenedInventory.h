#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenedInventory.generated.h"

UCLASS()
class PREPPER_API AOpenedInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	AOpenedInventory();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* InventoryMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCustomCameraComponent* InventoryCam;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = Camera)
	float DefaultCamArmLength;
};
