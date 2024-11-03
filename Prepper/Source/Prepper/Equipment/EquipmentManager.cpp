#include "EquipmentManager.h"

#include "Equipment.h"
#include "EquipmentData/EquipmentData.h"

EquipmentManager::EquipmentManager()
{
	/*
	TObjectPtr<UDataTable> WeaponDataTable;
	DataTableGetter::GetDataTable("WeaponDataTable", WeaponDataTable);

	Initial(WeaponDataTable);
	*/
}

EquipmentManager::~EquipmentManager()
{
}

void EquipmentManager::Initial(const TObjectPtr<UDataTable> EquipmentDataTable)
{
	if (!EquipmentDataTable) return;
	
	TArray<FEquipmentData*> Arr;
	EquipmentDataTable->GetAllRows(TEXT("GetAllRows"), Arr);
		
	for (int i = 0; i < Arr.Num(); ++i)
	{
		EquipmentData.Add(Arr[i]->EquipmentCode, Arr[i]->EquipmentClass);
	}
	UE_LOG(LogTemp, Warning, TEXT("EquipmentDataTableLoad: %d"), Arr.Num());
}

template <typename T>
TObjectPtr<T> EquipmentManager::SpawnWeapon(UWorld* World, const FString& WeaponCode)
{
	if (World == nullptr) return nullptr;
	if (!EquipmentData.Contains(WeaponCode)) return nullptr;
	return World->SpawnActor<T>(*EquipmentData.Find(WeaponCode));
	
}
