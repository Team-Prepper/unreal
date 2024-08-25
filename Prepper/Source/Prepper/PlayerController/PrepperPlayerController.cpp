#include "PrepperPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Prepper/Car/CarPawn.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/HUD/UI/CharacterOverlay/CharacterOverlay.h"
#include "Prepper/HUD/UI/Compass.h"
#include "Prepper/HUD/PrepperHUD.h"

void APrepperPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	PrepperHUD = Cast<APrepperHUD>(GetHUD());
}

void APrepperPlayerController::PossessPawn()
{
	Super::PossessPawn();
	PollInit();
}

void APrepperPlayerController::PollInit()
{
	return;
	if(!IsLocalController()) return;
	if(Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	}

	if(PlayerCharacter && PlayerCharacter->GetStatusEffectComponent())
	{
		PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
	}

	if (!PrepperHUD) return;
	PrepperHUD->ResetCrossHair();
	
	if(PrepperHUD->CharacterOverlay)
	{
		CharacterOverlay = PrepperHUD->CharacterOverlay;
	}

	if(PrepperHUD->Compass)
	{
		Compass = PrepperHUD->Compass;
		if(Compass)
		{
			UE_LOG(LogTemp,Warning,TEXT("[PrepperPlayerController] : Set Compass"));
			//SetCompass();
		}
	}
}

void APrepperPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrepperPlayerController::SetHUDStatusEffect(float Hunger, float Thirst, float Infection)
{
}