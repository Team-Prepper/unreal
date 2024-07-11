#include "InventoryInteractableItem.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/HUD/UI/InventoryInteractionUI.h"

AInventoryInteractableItem::AInventoryInteractableItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInventoryInteractableItem::SetTargetInventory(UMapInventory* Inventory)
{
	TargetInventory = Inventory;
}

void AInventoryInteractableItem::Interaction(APlayerCharacter* Target)
{
	if(TargetInventory->TryUseItem(ItemCode))
	{
		ItemManager::GetInstance()->GetItem(ItemCode)->Use(Target);
		DestroyInteractionItem();
	}
}

void AInventoryInteractableItem::ShowPickUpWidget(bool bShowWidget)
{
	PickUpWidget->SetVisibility(bShowWidget);
}


void AInventoryInteractableItem::InitializeWidget()
{
	if (PickUpWidget && PickUpWidget->GetUserWidgetObject())
	{
		UUserWidget* Widget = PickUpWidget->GetUserWidgetObject();
		if (Widget)
		{
			UInventoryInteractionUI* ItemInfoWidget = Cast<UInventoryInteractionUI>(Widget);
			if (ItemInfoWidget)
			{
				ItemInfoWidget->InitializeWidget(ItemCode);
			}
		}
	}
}
