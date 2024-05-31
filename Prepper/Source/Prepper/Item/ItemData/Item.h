// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prepper/Interfaces/ItemEffect.h"
#include "Prepper/Interfaces/PlayerAbility.h"
#include "Item.generated.h"

class HealEffect : public IItemEffect
{
	int Value;
public:
	HealEffect(int value)
	{
		Value = value;
	}
	virtual void Action(IPlayerAbility* Target) override
	{
		UE_LOG(LogTemp, Warning, TEXT("Heal:%d"), Value);
		//Target.
	}
};

class EatEffect : public IItemEffect
{
	int Value;
public:
	EatEffect(int value)
	{
		Value = value;
	}
	virtual void Action(IPlayerAbility* Target) override
	{
		UE_LOG(LogTemp, Warning, TEXT("Eat:%d"), Value);
		//
	} 
};

class DrinkEffect : public IItemEffect
{
	int Value;
public:
	DrinkEffect(int value)
	{
		Value = value;
	}
	virtual void Action(IPlayerAbility* Target) override
	{
		UE_LOG(LogTemp, Warning, TEXT("Drink:%d"), Value);
		//
	} 
};

USTRUCT()
struct PREPPER_API FItem
{
	GENERATED_BODY()

	TArray<IItemEffect *> ItemEffects;
	static IItemEffect* StringToItemEffect(const FString& Value)
	{
		
		TArray<FString> CodePiece; 
		const TCHAR* Delims[] = { TEXT(":") };
		const int32 nArraySize = Value.ParseIntoArray(CodePiece, Delims, 1);

		if (CodePiece[0].Compare("Heal"))
			return new HealEffect(FCString::Atoi(*CodePiece[1]));
		if (CodePiece[0].Compare("Eat"))
			return new EatEffect(FCString::Atoi(*CodePiece[1]));
		if (CodePiece[0].Compare("Drink"))
			return new DrinkEffect(FCString::Atoi(*CodePiece[1]));

		return new class HealEffect(FCString::Atoi(*CodePiece[1]));
		
	}
public:
	FString ItemCode;
	FText ItemName;
	UTexture2D* ItemIcon;
	FItem()
	{
		ItemCode = "";
		ItemName = FText::FromString("");
		ItemIcon = nullptr;
		
	}
	FItem(const FString& Code, const FText& Name, const FString& ItemEffectCode, UTexture2D* Icon)
	{
		ItemCode = Code;
		ItemName = Name;
		ItemIcon = Icon;
		
		TArray<FString> CodePiece; 
		const TCHAR* Delims[] = { TEXT(",") };
		const int32 nArraySize = ItemEffectCode.ParseIntoArray(CodePiece, Delims, 1);

		for (int i = 0; i < nArraySize; i++)
		{
			ItemEffects.Add(StringToItemEffect(CodePiece[i]));
		}
	}
	void Use(IPlayerAbility* Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Use"));
		for (IItemEffect* Effect : ItemEffects)
		{
			Effect->Action(Target);
		}
	}
};
