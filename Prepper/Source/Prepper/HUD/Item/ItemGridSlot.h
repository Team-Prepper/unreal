#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemGridSlot.generated.h"

class UButton;
class UProgressBar;
class UImage;
class UTextBlock;

UCLASS()
class PREPPER_API UItemGridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UMainInventoryHUD* MainHUD;
private:
	class IInventory* TargetInventory;
	FString ItemCode;
	UPROPERTY()
	class UPrepperGameInstance* PrepperGameInstance;
	

	UPROPERTY()
	class UItemUIData* Data;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCount;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ItemCountBar;
	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;


	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UUserWidget> ItemInfoClass;
	UPROPERTY()
	class UItemGridSlotInfo* ItemInfo;
	
public:
	void SetupSlot(FString TargetItemCode, uint32 SetItemCount);

	void SetInventory(IInventory* Target);

	UFUNCTION(BlueprintCallable)
	void SlotButtonPressed();
};
