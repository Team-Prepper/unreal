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

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;
	
	UPROPERTY(EditAnywhere)
	int SlotsPerRow = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemSlotClass;

	TArray<UItemGridSlot*> GridSlots;

	IInventory* TargetInventory;
	
	void UpdateData();
public:
	virtual void NativeConstruct() override;

	
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	void Set(IInventory* Target);

protected:
	void AddSlot(int Capacity);
	void AddSlotsToGrid(int Index, UItemGridSlot* Slot);
};
