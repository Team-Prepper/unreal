#include "CharacterOverlay.h"

#include "Components/Image.h"
#include "Components/VerticalBox.h"

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

void UCharacterOverlay::Update(const TMap<FString, uint8>& NewData)
{
	ClearAllItemIcons();
	for (const TPair<FString, uint8>& Elem : NewData)
	{
		// 키와 값 가져오기
		const FString& Key = Elem.Key;
		uint8 Value = Elem.Value;

		// 로그로 출력
		UE_LOG(LogTemp, Log, TEXT("Key: %s, Value: %d"), *Key, Value);

		// UI에 이미지 추가하기
		
		AddItemIcon(ItemManager::GetInstance()->GetItem(Key)->ItemIcon);
	}
}

void UCharacterOverlay::AddItemIcon(UTexture2D* ItemIconTexture)
{
	if (ItemBox && ItemIconTexture)
	{
		// 새로운 UImage 위젯 생성
		UImage* NewItemIcon = NewObject<UImage>(this);

		// UImage에 텍스처 설정
		NewItemIcon->SetBrushFromTexture(ItemIconTexture);

		// Vertical Box에 추가
		ItemBox->AddChild(NewItemIcon);
	}
}

void UCharacterOverlay::ClearAllItemIcons()
{
	if (ItemBox)
	{
		ItemBox->ClearChildren();
	}
}
