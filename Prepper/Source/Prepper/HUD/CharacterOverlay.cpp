#include "CharacterOverlay.h"

#include "Prepper/Item/InventoryUI.h"

void UCharacterOverlay::SetInventoryVisible(bool IsVisible)
{
	if(!InventoryHUD) return;

	if(IsVisible)
	{
		InventoryHUD->SetVisibility(ESlateVisibility::Visible);
		
	}
	else
	{
		InventoryHUD->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UCharacterOverlay::SetInventory(IInventory* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory Setting"));
	Cast<UInventoryUI>(InventoryHUD)->Set(Target);
}
