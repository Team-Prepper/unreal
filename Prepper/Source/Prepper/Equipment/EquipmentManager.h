#pragma once

#include "Equipment.h"
#include "Prepper/_Base/Singleton.h"

class AEquipment;
struct FEquipmentData;

class EquipmentManager : public Singleton<EquipmentManager>
{
public:
	TMap<FString, TSubclassOf<AEquipment>> EquipmentData;
public:
	EquipmentManager();
	virtual ~EquipmentManager() override;
	void Initial(const TObjectPtr<UDataTable> EquipmentDataTable);

	template <typename T>
	TObjectPtr<T> SpawnEquipment(UWorld* World, const FString& WeaponCode);
};

template <typename T>
TObjectPtr<T> EquipmentManager::SpawnEquipment(UWorld* World, const FString& WeaponCode)
{
	if (World == nullptr) return nullptr;
	if (!EquipmentData.Contains(WeaponCode)) return nullptr;
	return World->SpawnActor<T>(*EquipmentData.Find(WeaponCode));
}