#pragma once

#include "Prepper/_Base/Singleton.h"

class AWeaponActor;
struct FWeaponData;

class WeaponManager : public Singleton<WeaponManager>
{
public:
	TMap<FString, TSubclassOf<AWeaponActor>> WeaponData;
public:
	WeaponManager();
	virtual ~WeaponManager() override;
	void Initial(const TObjectPtr<UDataTable> WeaponDataTable);
	
	TObjectPtr<AWeaponActor> SpawnWeapon(UWorld* World, const FString& WeaponCode);
	
};
