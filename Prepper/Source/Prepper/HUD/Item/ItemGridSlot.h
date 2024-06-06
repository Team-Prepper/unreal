#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemGridSlot.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PREPPER_API UItemGridSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	class IInventory* TargetInventory;
	FString ItemCode;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;

	class UItemGridSlotInfo* ItemInfo;
};
