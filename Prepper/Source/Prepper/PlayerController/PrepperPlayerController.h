#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "PrepperPlayerController.generated.h"

class UTexture2D;

UCLASS()
class PREPPER_API APrepperPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PollInit() override;
	
public:
	virtual void PossessPawn() override;
	virtual void Tick(float DeltaTime) override;

	/* Set HUD*/
	void SetHUDHealth(float Health, float MaxHealth);

	void SetHUDStatusEffect(float Hunger, float Thirst, float Infection);

	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> WeaponTierTextures;
	void SetHUDWeaponTier(uint8 Value);
	void SetHUDWeaponAmmo(int32 Value);
	void SetHUDCarriedAmmo(int32 Value);
	void SetCompass();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	UPROPERTY()
	class UCompass* Compass;
	
	UPROPERTY()
	class UCameraComponent* PlayerCam;
	
	bool bInitCharacterOverlay = false;
	
	float HUDHealth;
	float HUDMaxHealth;
};
