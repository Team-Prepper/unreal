#include "UIInventoryViewer.h"

UIInventoryViewer::UIInventoryViewer(APrepperHUD* HUD)
{
	PrepperHUD = HUD;
	
}

void UIInventoryViewer::ToggleInventory()
{
	if (!PrepperHUD) return;
	PrepperHUD->ToggleInventory();
}
