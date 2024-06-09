#include "ItemGridSlot.h"
#include "ItemGridSlotInfo.h"
#include "ItemUIData.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MainInventoryHUD.h"
#include "Prepper/GameInstance/PrepperGameInstance.h"

void UItemGridSlot::SetupSlot(FString TargetItemCode, uint32 SetItemCount)
{
	PrepperGameInstance = Cast<UPrepperGameInstance>(GetGameInstance());

	Data = NewObject<UItemUIData>(GetWorld(), UItemUIData::StaticClass());
	
	if (!PrepperGameInstance->ItemData.GetItemData(TargetItemCode, Data->TextureIcon, Data->ItemName))
	{
		UE_LOG(LogTemp,Warning, TEXT("No Item Data"));
		return;
	}

	ItemIcon->SetBrushFromTexture(Data->TextureIcon);
	ItemName->SetText(Data->ItemName);
	ItemCode = TargetItemCode;
	FString CountStr;
	CountStr = FString::Printf(TEXT("%d"),SetItemCount);
	ItemCount->SetText(FText::FromString(CountStr));
	ItemCountBar->SetVisibility(ESlateVisibility::Visible);
	ItemCountBar->SetPercent(SetItemCount / 100);
}

void UItemGridSlot::SetInventory(IInventory* Target)
{
	TargetInventory = Target;
}

void UItemGridSlot::SlotButtonPressed()
{
	if(!MainHUD) return;

	MainHUD->ItemInfoIcon->SetBrushFromTexture(Data->TextureIcon);
	MainHUD->ItemInfoName->SetText(Data->ItemName);
	MainHUD->SelectItemCode = ItemCode;
}
