#include "CharacterOverlay.h"

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
