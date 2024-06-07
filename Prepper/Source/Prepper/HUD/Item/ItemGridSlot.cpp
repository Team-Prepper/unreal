#include "ItemGridSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemGridSlot::SetIcon(UTexture2D*& SetIconImg, FText SetItemName, uint32 SetItemCount)
{
	ItemIcon->SetBrushFromTexture(SetIconImg);
	ItemName->SetText(SetItemName);

	FString CountStr;
	CountStr = FString::Printf(TEXT("%d"),SetItemCount);
	ItemCount->SetText(FText::FromString(CountStr));
}