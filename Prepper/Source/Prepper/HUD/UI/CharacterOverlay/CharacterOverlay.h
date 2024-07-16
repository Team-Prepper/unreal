#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Prepper/_Base/ObserverPattern/Observer.h"
#include "Prepper/_Base/Util/GaugeValue.h"
#include "CharacterOverlay.generated.h"

UCLASS()
class PREPPER_API UCharacterOverlay : public UUserWidget, public IObserver<GaugeValue<float>>, public IObserver<GaugeValue<int>>
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefeatsValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountDownText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HungerBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ThirstBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* InfectionBar;

	virtual void Update(const GaugeValue<float>& NewData) override;
	virtual void Update(const GaugeValue<int>& NewData) override;
	
};
