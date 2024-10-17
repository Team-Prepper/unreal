// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Character/Component/Combat/BaseCombatComponent.h"
#include "Prepper/Character/Component/Combat/CombatComponent.h"
#include "Prepper/GameSave/SurvivorSaveGame.h"
#include "Prepper/Mission/MissionChecker/DefaultMissionChecker.h"
#include "Prepper/Weapon/WeaponActor.h"
#include "Prepper/Weapon/WeaponManager.h"

void ASurvivorGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, ABasePlayerController* VictimController,
                                         ABasePlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	
}

void ASurvivorGameMode::SaveGame()
{
	Super::SaveGame();
	
}

void ASurvivorGameMode::SavePlayerData(const APlayerCharacter* TargetPlayerCharacter)
{
	const TObjectPtr<USurvivorSaveGame> SaveGameInstance =
		Cast<USurvivorSaveGame>(UGameplayStatics::CreateSaveGameObject(USurvivorSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		UCombatComponent* CombatComp = Cast<UCombatComponent>(TargetPlayerCharacter->GetCombatComponent());

		if (CombatComp)
		{
			SaveGameInstance->EquippedWeapon =
				CombatComp->EquippedWeapon->GetWeaponCode();
			SaveGameInstance->SecondaryEquippedWeapon =
				CombatComp->SecondaryWeapon->GetWeaponCode();
		}

		TArray<IInventory::InventoryItem> ItemData = TargetPlayerCharacter->GetInventory()->GetIter();
		for (int i = 0; i < ItemData.Num(); i++)
		{
			SaveGameInstance->InventoryItemCode.Add(ItemData[i].ItemCode);
			SaveGameInstance->InventoryItemCount.Add(ItemData[i].Count);
		}
		
		TArray<IInventory::InventoryItem> QuickSlotData = TargetPlayerCharacter->GetInventory()->GetIter();
		for (int i = 0; i < QuickSlotData.Num(); i++)
		{
			SaveGameInstance->QuickSlotItemCode.Add(QuickSlotData[i].ItemCode);
			SaveGameInstance->QuickSlotItemCount.Add(QuickSlotData[i].Count);
		}
		SaveGameInstance->LastPosition = TargetPlayerCharacter->GetActorLocation();
		
	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Test", 0);
}

void ASurvivorGameMode::SaveServerData()
{
}

void ASurvivorGameMode::LoadGame(APlayerCharacter* TargetPlayerCharacter)
{
	USurvivorSaveGame* LoadGameInstance =
		Cast<USurvivorSaveGame>(UGameplayStatics::LoadGameFromSlot("Test", 0));

	if (LoadGameInstance)
	{
		TargetPlayerCharacter->SetActorLocation(LoadGameInstance->LastPosition);

		int QuickSlotIdx = 0;
		for (auto Item : LoadGameInstance->QuickSlotItemCode)
		{
			TargetPlayerCharacter->AddItem(Item, LoadGameInstance->QuickSlotItemCount[QuickSlotIdx]);
			TargetPlayerCharacter->GetInventory()->QuickSlotAdd(Item, QuickSlotIdx++);
		}
		int ItemIdx = 0;
		for (auto Item : LoadGameInstance->InventoryItemCode)
		{
			TargetPlayerCharacter->AddItem(Item, LoadGameInstance->InventoryItemCount[ItemIdx++]);
		}

		AWeaponActor* SpawnWeapon = 
			WeaponManager::GetInstance()->SpawnWeapon(GetWorld(), LoadGameInstance->EquippedWeapon);
		TargetPlayerCharacter->EquipWeapon(SpawnWeapon);
		
		AWeaponActor* SpawnWeapon2 =
			WeaponManager::GetInstance()->SpawnWeapon(GetWorld(), LoadGameInstance->SecondaryEquippedWeapon);
		
		TargetPlayerCharacter->EquipWeapon(SpawnWeapon2);
	}
}

void ASurvivorGameMode::AddMission()
{
	
}
