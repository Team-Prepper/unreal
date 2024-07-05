#pragma once
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/Interfaces/InventoryViewer.h"

class UIInventoryViewer : public IInventoryViewer
{
	APrepperHUD* PrepperHUD;
public:
	UIInventoryViewer(APrepperHUD* HUD);
	virtual void ToggleInventory() override;
};
