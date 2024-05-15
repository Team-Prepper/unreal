#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "PrepperPlayerController.generated.h"

UCLASS()
class PREPPER_API APrepperPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PollInit() override;
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	

	/* Set HUD*/
	void SetHUDHealth(float Health, float MaxHealth);
	
	void SetHUDWeaponAmmo(int32 Value);
	void SetHUDCarriedAmmo(int32 Value);

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	
	bool bInitCharacterOverlay = false;
	
	float HUDHealth;
	float HUDMaxHealth;
};
