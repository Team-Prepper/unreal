#pragma once

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
	TObjectPtr<T> SpawnWeapon(UWorld* World, const FString& WeaponCode);
	
};
