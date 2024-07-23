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
	virtual void PossessPawn() override;
	virtual void Tick(float DeltaTime) override;

	void SetHUDStatusEffect(float Hunger, float Thirst, float Infection);

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	UPROPERTY()
	class UCompass* Compass;
	
	UPROPERTY()
	UCameraComponent* PlayerCam;
	
	bool bInitCharacterOverlay = false;
	
	float HUDHealth;
	float HUDMaxHealth;
};
