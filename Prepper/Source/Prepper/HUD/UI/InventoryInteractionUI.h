#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryInteractionUI.generated.h"

UCLASS()
class PREPPER_API UInventoryInteractionUI : public UUserWidget
{
	GENERATED_BODY()
public:
	//meta = (BindWidget)
	
	FString ItemCode;
	
	UPROPERTY()
	class UImage* ItemIcon;

	UPROPERTY()
	class UTextBlock* ItemName;

	UPROPERTY()
	class UTextBlock* ItemDescription;

	UPROPERTY()
	class UButton* UseButton;

	UPROPERTY()
	class UButton* DiscardButton;

	void InitializeWidget(const FString& InitItemCode);
};
