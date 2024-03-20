#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableItem.generated.h"

UCLASS()
class PREPPER_API AInteractableItem : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractableItem();

	virtual void Interaction(APlayerCharacter *Target) override;
	virtual void ShowPickUpWidget(bool bShowWidget) override;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FString ItemCode;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	class UWidgetComponent* PickUpWidget;

};