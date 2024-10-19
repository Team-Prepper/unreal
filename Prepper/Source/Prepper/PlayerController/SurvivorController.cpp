// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorController.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/Character/Component/Combat/CombatComponent.h"
#include "Prepper/GameSave/SurvivorSaveGame.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/HUD/UI/CharacterOverlay/StatusWidget.h"
#include "Prepper/HUD/UI/Inventory/InventoryUI.h"
#include "Prepper/HUD/UI/ItemCombination/ItemCombinationUI.h"
#include "Prepper/Weapon/WeaponActor.h"
#include "Prepper/Weapon/WeaponManager.h"

void ASurvivorController::BeginWidget()
{
	Super::BeginWidget();
	
	if (StatusWidgetClass && StatusWidget == nullptr)
	{
		StatusWidget = CreateWidget<UStatusWidget>(this, StatusWidgetClass);
		StatusWidget->AddToViewport();
	}
	
	if (InventoryWidgetClass && InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UInventoryUI>(this, InventoryWidgetClass);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->AddToViewport();
	}
}

void ASurvivorController::PossessPlayerCharacter()
{
	Super::PossessPlayerCharacter();
	
	if (!PlayerCharacter) return;
	if (!CharacterOverlay) return;
	if (!StatusWidget) return;
	if (!PlayerCharacter->GetStatusEffectComponent()) return;
	
	LoadGame();

	UE_LOG(LogTemp, Warning, TEXT("Attach UI"));
	
	PlayerCharacter->GetStatusEffectComponent()->Attach(StatusWidget);
	PlayerCharacter->GetStatusEffectComponent()->StatusTimerStart();
	InventoryWidget->SetTargetPlayer(PlayerCharacter);
}

void ASurvivorController::SetInput(UEnhancedInputComponent* Input)
{
	Super::SetInput(Input);
	
	Input->BindAction(OpenInventory, ETriggerEvent::Triggered, this, &ASurvivorController::OpenInventoryPressed);
	Input->BindAction(Button1, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot1Use);
	Input->BindAction(Button2, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot2Use);
	Input->BindAction(Button3, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot3Use);
	Input->BindAction(Button4, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot4Use);
	Input->BindAction(Button5, ETriggerEvent::Started, this, &ASurvivorController::QuickSlot5Use);
	
}

void ASurvivorController::OpenInventoryPressed()
{
	if (!InventoryWidget) return;
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	//ServerToggleInventory();
}

void ASurvivorController::ServerToggleInventory_Implementation()
{
	/*
	if (!TargetControllerable) return;
	TargetControllerable->ToggleInventory();
	*/
}

void ASurvivorController::QuickSlot1Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button1Pressed"));
	
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(0);
	
	return;
	if (ItemCombinationClass == nullptr) return;
	TObjectPtr<UItemCombinationUI> Tmp =
		CreateWidget<UItemCombinationUI>(this, ItemCombinationClass);
	
	Tmp->SetTargetPlayer(PlayerCharacter);
	Tmp->SetVisibility(ESlateVisibility::Visible);
	Tmp->AddToViewport();
}

void ASurvivorController::QuickSlot2Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button2Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(1);
}

void ASurvivorController::QuickSlot3Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button3Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(2);
}

void ASurvivorController::QuickSlot4Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button4Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(3);
}

void ASurvivorController::QuickSlot5Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Button5Pressed"));
	if (!PlayerCharacter) return;
	PlayerCharacter->UseQuickSlotItem(4);
}

void ASurvivorController::LoadGame()
{
	USurvivorSaveGame* LoadGameInstance =
		Cast<USurvivorSaveGame>(UGameplayStatics::LoadGameFromSlot("Test", 0));

	if (LoadGameInstance)
	{
		PlayerCharacter->SetActorLocation(LoadGameInstance->LastPosition);

		int QuickSlotIdx = 0;
		for (auto Item : LoadGameInstance->QuickSlotItemCode)
		{
			if (LoadGameInstance->QuickSlotItemCount[QuickSlotIdx] < 1) continue;
			ServerAddItem(Item, LoadGameInstance->QuickSlotItemCount[QuickSlotIdx]);
			PlayerCharacter->GetInventory()->QuickSlotAdd(Item, QuickSlotIdx++);
		}
		int ItemIdx = 0;
		for (auto Item : LoadGameInstance->InventoryItemCode)
		{
			ServerAddItem(Item, LoadGameInstance->InventoryItemCount[ItemIdx++]);
		}
		
		for (auto Value : LoadGameInstance->CarriedAmmoMap)
		{
			ServerSetAmmo(Value.Key, Value.Value);
		}

		ServerEquipWeapon(LoadGameInstance->EquippedWeapon);
		ServerEquipWeapon(LoadGameInstance->SecondaryEquippedWeapon);
		
	}
}

void ASurvivorController::ServerAddItem_Implementation(const FString& ItemCode, int Count)
{
	PlayerCharacter->GetInventory()->TryAddItem(ItemCode, Count);
}

void ASurvivorController::ServerSetAmmo_Implementation(EWeaponType Type, int Count)
{
	UCombatComponent* Target =
			Cast<UCombatComponent>(PlayerCharacter->GetCombatComponent());

	if (Target == nullptr) return;
	
	if (Target->CarriedAmmoMap.Contains(Type))
	{
		Target->CarriedAmmoMap[Type] = Count;
		return;
	}
	
	Target->CarriedAmmoMap.Add(Type, Count);
	
}

void ASurvivorController::ServerEquipWeapon_Implementation(const FString& WeaponCode)
{
	AWeaponActor* SpawnWeapon =
		WeaponManager::GetInstance()->SpawnWeapon(GetWorld(), WeaponCode);
		
	PlayerCharacter->EquipWeapon(SpawnWeapon);
	
}


void ASurvivorController::SaveGame()
{
	const TObjectPtr<USurvivorSaveGame> SaveGameInstance =
		Cast<USurvivorSaveGame>(UGameplayStatics::CreateSaveGameObject(USurvivorSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		
		if (UCombatComponent* CombatComp =
			Cast<UCombatComponent>(PlayerCharacter->GetCombatComponent()))
		{
			if (CombatComp->EquippedWeapon != nullptr)
			{
				SaveGameInstance->EquippedWeapon =
					CombatComp->EquippedWeapon->GetWeaponCode();
			}
			if (CombatComp->SecondaryWeapon != nullptr)
			{
				SaveGameInstance->SecondaryEquippedWeapon =
					CombatComp->SecondaryWeapon->GetWeaponCode();
			}

			SaveGameInstance->CarriedAmmoMap = CombatComp->CarriedAmmoMap;
		}

		TArray<FItemConvertData> ItemData = PlayerCharacter->GetInventory()->GetIter();
		for (int i = 0; i < ItemData.Num(); i++)
		{
			SaveGameInstance->InventoryItemCode.Add(ItemData[i].ItemCode);
			SaveGameInstance->InventoryItemCount.Add(ItemData[i].Count);
		}
		
		TArray<FItemConvertData> QuickSlotData = PlayerCharacter->GetInventory()->GetQuickSlotIter();
		for (int i = 0; i < QuickSlotData.Num(); i++)
		{
			SaveGameInstance->QuickSlotItemCode.Add(QuickSlotData[i].ItemCode);
			SaveGameInstance->QuickSlotItemCount.Add(QuickSlotData[i].Count);
		}
		SaveGameInstance->LastPosition = PlayerCharacter->GetActorLocation();
		
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Test", 0);
	
}
