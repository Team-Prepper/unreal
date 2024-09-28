// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "UI/CharacterOverlay/CharacterOverlay.h"
#include "PrepperHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
	
	UTexture2D* CrosshairCenter;
	UTexture2D* CrosshairLeft;
	UTexture2D* CrosshairRight;
	UTexture2D* CrosshairTop;
	UTexture2D* CrosshairBottom;
	
	float CrosshairSpread;

	FLinearColor CrosshairColor;
	
};

UCLASS()
class PREPPER_API APrepperHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	void AddCharacterOverlay();

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	UPROPERTY()
	class UCompass* Compass;
	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddWidget(TSubclassOf<UUserWidget> Widget);
	void AddAnnouncement();

	UFUNCTION()
	void ResetCrossHair();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UUserWidget> InventoryHUDClass;
	
	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UUserWidget> CompassHUDClass;
	
	UPROPERTY(EditAnywhere, Category = "Annoucement")
	TSubclassOf<UUserWidget> AnnouncementClass;
	
	FHUDPackage HUDPackage;
	bool IsInventoryVisible = false;
	
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	void DrawCrosshair();
	void DrawCrosshairUnit(UTexture2D* Texture, const FVector2D& ViewportCenter, const FVector2D& Spread, const FLinearColor& CrosshairColor);

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
};
