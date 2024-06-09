#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Prepper/Interfaces/Inventory.h"
#include "ItemGrid.generated.h"

class UItemGridSlot;

UCLASS()
class PREPPER_API UItemGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class UMainInventoryHUD* MainHUD;

	TArray<UItemGridSlot*> GridSlots;
	
	void UpdateData();
protected:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;

	UPROPERTY(EditAnywhere)
	int InventoryCapacity = 30;
	
	UPROPERTY(EditAnywhere)
	int SlotsPerRow = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemSlotClass;

	

	IInventory* TargetInventory;
	
	
	
public:
	virtual void NativeConstruct() override;
	void Set(IInventory* Target);

protected:
	void AddSlot(int Capacity);
	void AddSlotsToGrid(int Index, UItemGridSlot* Slot);
};
