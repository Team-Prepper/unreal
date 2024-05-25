#include "PrepperPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Prepper/Car/CarPawn.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Component/StatusEffectComponent.h"
#include "Prepper/HUD/CharacterOverlay.h"
#include "Prepper/HUD/Compass.h"
#include "Prepper/HUD/PrepperHUD.h"

void APrepperPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PrepperHUD = Cast<APrepperHUD>(GetHUD());
	
}

void APrepperPlayerController::PossessPawn()
{
	Super::PossessPawn();
	PollInit();
}

void APrepperPlayerController::PollInit()
{
	if(!IsLocalController()) return;
	if(Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	}
	if(PrepperHUD && PrepperHUD->CharacterOverlay)
	{
		PrepperHUD->ResetCrossHair();
		CharacterOverlay = PrepperHUD->CharacterOverlay;
		if(CharacterOverlay && PlayerCharacter)
		{
			UE_LOG(LogTemp,Warning,TEXT("[PrepperPlayerController] : Set CharacterOverlay"));
			UE_LOG(LogTemp,Warning,TEXT("%f, %f"),PlayerCharacter->GetCurrentHealth(),PlayerCharacter->GetMaxHealth());
			SetHUDHealth(PlayerCharacter->GetCurrentHealth(),PlayerCharacter->GetMaxHealth());
		}
	}

	if(PrepperHUD && PrepperHUD->Compass)
	{
		Compass = PrepperHUD->Compass;
		if(Compass)
		{
			UE_LOG(LogTemp,Warning,TEXT("[PrepperPlayerController] : Set Compass"));
			SetCompass();
		}
	}

	if(PlayerCharacter && PlayerCharacter->GetStatusEffectComponent())
	{
		PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
	}
}

void APrepperPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Compass)
	{
		Compass->SetDirection();
	}
}


void APrepperPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	if(!IsLocalController()) return;
	// 주로 폰이 변경되었을 떄 사용 (init,차량 탑승, 부활 등)
	// 데미지 처리시 HUD 갱신은 Player에서 처리중
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->HealthBar &&
					 PrepperHUD->CharacterOverlay->HealthText;
	
	if(bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		PrepperHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"),FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		PrepperHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void APrepperPlayerController::SetHUDStatusEffect(float Hunger, float Thirst, float Infection)
{
	if(!IsLocalController()) return;
	
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->HungerBar &&
					 PrepperHUD->CharacterOverlay->ThirstBar &&
					 PrepperHUD->CharacterOverlay->InfectionBar;
	if(bHUDValid)
	{
		PrepperHUD->CharacterOverlay->HungerBar->SetPercent(Hunger / 100);
		PrepperHUD->CharacterOverlay->ThirstBar->SetPercent(Thirst/ 100);
		PrepperHUD->CharacterOverlay->InfectionBar->SetPercent(Infection/ 100);
	}
}

void APrepperPlayerController::SetHUDWeaponAmmo(int32 Value)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->WeaponAmmoValue;
	if(bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"),Value);
		PrepperHUD->CharacterOverlay->WeaponAmmoValue->SetText(FText::FromString(AmmoText));
	}
}

void APrepperPlayerController::SetHUDCarriedAmmo(int32 Value)
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;
	bool bHUDValid = PrepperHUD &&
					 PrepperHUD->CharacterOverlay &&
					 PrepperHUD->CharacterOverlay->CarriedAmmoValue;
	if(!bHUDValid) return;
	
	FString AmmoText = FString::Printf(TEXT("%d"),Value);
	PrepperHUD->CharacterOverlay->CarriedAmmoValue->SetText(FText::FromString(AmmoText));
}

void APrepperPlayerController::SetCompass()
{
	if (Cast<IControllable>(GetPawn()) == nullptr) return;
	Compass->PlayerCam = Cast<IControllable>(GetPawn())->GetFollowCamera();
}
