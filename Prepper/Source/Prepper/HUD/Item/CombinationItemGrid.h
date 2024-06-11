#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombinationItemGrid.generated.h"

class UCombinationItemSlot;
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class PREPPER_API UCombinationItemGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class UItemCombineUI* CombineUI;
	
	TArray<UCombinationItemSlot*> GridSlots;

protected:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;

	UPROPERTY(EditAnywhere)
	int SlotsPerRow = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ItemSlotClass;

public:
	virtual void NativeConstruct() override;
	void AddSlot();
protected:
	void AddSlotsToGrid(int Index, UCombinationItemSlot* NewSlot);
};
