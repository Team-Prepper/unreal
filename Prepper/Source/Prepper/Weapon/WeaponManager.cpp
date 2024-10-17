#include "WeaponManager.h"
#include "WeaponData/WeaponData.h"
#include "WeaponActor.h"
#include "Prepper/_Base/DataTableGetter.h"

WeaponManager::WeaponManager()
{
	/*
	TObjectPtr<UDataTable> WeaponDataTable;
	DataTableGetter::GetDataTable("WeaponDataTable", WeaponDataTable);

	Initial(WeaponDataTable);
	*/
}

WeaponManager::~WeaponManager()
{
}

void WeaponManager::Initial(const TObjectPtr<UDataTable> WeaponDataTable)
{
	if (!WeaponDataTable) return;
	
	TArray<FWeaponData*> Arr;
	WeaponDataTable->GetAllRows(TEXT("GetAllRows"), Arr);
		
	for (int i = 0; i < Arr.Num(); ++i)
	{
		WeaponData.Add(Arr[i]->WeaponCode, Arr[i]->WeaponClass);
	}
	UE_LOG(LogTemp, Warning, TEXT("WeaponDataTableLoad: %d"), Arr.Num());
}

TObjectPtr<AWeaponActor> WeaponManager::SpawnWeapon(UWorld* World, const FString& WeaponCode)
{
	if (World == nullptr) return nullptr;
	if (!WeaponData.Contains(WeaponCode)) return nullptr;
	return World->SpawnActor<AWeaponActor>(*WeaponData.Find(WeaponCode));
}
