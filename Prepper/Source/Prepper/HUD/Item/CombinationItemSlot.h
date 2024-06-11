#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombinationItemSlot.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class PREPPER_API UCombinationItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UItemCombineUI* CombineUI;

	void SetSlot(const FString& CombinedItemCode, UItemCombineUI* CombineMainHUD);
private:
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
	UButton* SlotButton;

	UFUNCTION(BlueprintCallable)
	void SlotButtonPressed();
};
