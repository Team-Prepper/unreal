#include "CharacterOverlay.h"


void UCharacterOverlay::Update(const GaugeValue<float>& NewData)
{
	HealthBar->SetPercent(NewData.GetRatio());
	
	const FString Text = FString::Printf(TEXT("%d/%d"),
		FMath::CeilToInt(NewData.GetCurValue()), FMath::CeilToInt(NewData.GetMaxValue()));
	HealthText->SetText(FText::FromString(Text));

	if (NewData.GetCurValue() > 0.0f) return;

	const FString Minus1 = FString::Printf(TEXT("%d"), -1);
	WeaponAmmoValue->SetText(FText::FromString(Minus1));
	CarriedAmmoValue->SetText(FText::FromString(Minus1));
	
}

void UCharacterOverlay::Update(const GaugeValue<int>& NewData)
{
	const FString AmmoText = FString::Printf(TEXT("%d"), NewData.GetCurValue());
	WeaponAmmoValue->SetText(FText::FromString(AmmoText));
	
	const FString CarriedText = FString::Printf(TEXT("%d"), NewData.GetMaxValue());
	CarriedAmmoValue->SetText(FText::FromString(CarriedText));
}

void UCharacterOverlay::Update(const Status& NewData)
{
	HungerBar->SetPercent(NewData.Hungry.GetRatio());
	ThirstBar->SetPercent(NewData.Thirsty.GetRatio());
	InfectionBar->SetPercent(NewData.Infected.GetRatio());
}
