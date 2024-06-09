#include "PrepperHUD.h"

#include "Announcement.h"
#include "CharacterOverlay.h"
#include "Compass.h"
#include "Prepper/HUD/Item/ItemGrid.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Item/ItemCombineUI.h"
#include "Item/MainInventoryHUD.h"
#include "Prepper/Character/PlayerCharacter.h"

void APrepperHUD::BeginPlay()
{
	Super::BeginPlay();
}

void APrepperHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController)
	{
		if(CharacterOverlayClass)
		{
			CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
			CharacterOverlay->AddToViewport();
		}
		if(InventoryHUDClass)
		{
			InventoryHUD = CreateWidget<UMainInventoryHUD>(PlayerController, InventoryHUDClass);
			InventoryHUD->AddToViewport();
			InventoryHUD->SetVisibility(ESlateVisibility::Hidden);
			if(InventoryHUD->ItemGrid)
				InventoryHUD->ItemGrid->Set(&Cast<APlayerCharacter>(GetOwningPawn())->Inven);
		}
		if(CraftingHUDClass)
		{
			ItemCombineUI = CreateWidget<UItemCombineUI>(PlayerController, CraftingHUDClass);
			ItemCombineUI->AddToViewport();
			ItemCombineUI->SetVisibility(ESlateVisibility::Hidden);
		}
		if(CompassHUDClass)
		{
			Compass = CreateWidget<UCompass>(PlayerController, CompassHUDClass);
			Compass->AddToViewport();
		}
	}
}

void APrepperHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

void APrepperHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if(GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X/ 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if(HUDPackage.CrosshairCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(HUDPackage.CrosshairLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(HUDPackage.CrosshairRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(HUDPackage.CrosshairTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}

		if(HUDPackage.CrosshairBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
	}
}

void APrepperHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight =  Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
	ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
	ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
		);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureWidth,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
		);
}

void APrepperHUD::ToggleInventory()
{
	if(!InventoryHUD) return;
	InventoryHUD->SetInventory(&Cast<APlayerCharacter>(GetOwningPawn())->Inven);
	IsInventoryVisible = !IsInventoryVisible;
	InventoryHUD->SetVisibility(IsInventoryVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	GetOwningPlayerController()->SetShowMouseCursor(IsInventoryVisible);
	if(IsInventoryVisible)
	{
		GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
	}
	
}

void APrepperHUD::ResetCrossHair()
{
	HUDPackage.CrosshairCenter = nullptr;
	HUDPackage.CrosshairLeft = nullptr;
	HUDPackage.CrosshairRight = nullptr;
	HUDPackage.CrosshairTop = nullptr;
	HUDPackage.CrosshairBottom = nullptr;
}
