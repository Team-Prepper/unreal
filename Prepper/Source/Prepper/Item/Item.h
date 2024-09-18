// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/ItemEffect.h"
#include "GameFramework/Actor.h"
#include "Object/InventoryInteractableItem.h"
#include "Prepper/Interfaces/PlayerAbility.h"
#include "Item.generated.h"



USTRUCT()
struct PREPPER_API FItem
{
	GENERATED_BODY()

	FString ItemCode;
	FText ItemName;

	UPROPERTY()
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY()
	TSubclassOf<AActor> ItemObject;
	TSubclassOf<AInventoryInteractableItem> ItemInteraction;

	TArray<IItemEffect *> ItemEffects;
	static IItemEffect* StringToItemEffect(const FString& Value);
	
	FItem();
	FItem(const FString& Code, const FText& Name, const FString& ItemEffectCode, UTexture2D* Icon, TSubclassOf<AActor> Object, TSubclassOf<AInventoryInteractableItem> Interaction);
	
	void Use(IPlayerAbility* Target);
};
