// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "UObject/ConstructorHelpers.h"

bool Inventory::TryCombineItem(const FString& Input1, const FString& Input2, FString& Result)
{
	return  ItemData.TryCombinationItem(Input1, Input2, Result);
}

Inventory::Inventory()
{
	BulletCount = 0;
}

Inventory::~Inventory()
{
	
}

bool Inventory::TryAddItem(const FString& ItemCode)
{
	FString retval;
	
	if (TryCombineItem("Milk", "Milgaru", retval))
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Test Success: %s"), *retval);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail Test"));
	}
	
	// 아이템이 존재한다면
	// 그 아이템의 소지수를 1 늘리고 true 반환
	if (ItemUnits.Contains(ItemCode))
	{
		const uint8 ItemCount = *ItemUnits.Find(ItemCode) + 1;
		ItemUnits.Add(ItemCode, ItemCount);
		
		UE_LOG(LogTemp, Warning, TEXT("Add Item %s"), *ItemCode);
		
	}

	// 새로운 아이템이 들어갈 공간이 없다면  false 반환
	if (ItemUnits.Num() >= MAX_ITEM_COUNT)
	{
		return false;
	}
	
	// 새로운 아이템을 추가하고 true 반환
	ItemUnits.Add(ItemCode, 1);
	UE_LOG(LogTemp, Warning, TEXT("Add Item %s"), *ItemCode);
	
	return true;
}

bool Inventory::TryUseItem(const FString& ItemCode)
{
	// 아이템이 존재하지 않는다면 return false
	if (!ItemUnits.Contains(ItemCode))	return false;

	const uint8 ItemCount = *ItemUnits.Find(ItemCode) - 1;

	// 아이템 사용 후의 개수가 0인 경우 삭제
	if (ItemCount == 0)
	{
		ItemUnits.Remove(ItemCode);	
	}
	// 아닌 경우 아이템의 소주 수를 -1
	else
	{
		ItemUnits.Add(ItemCode, ItemCount);
	}

	// 아이템 사용에 성공했으므로 true 반환
	return true;
}

void Inventory::AddBullet(const uint8 Count)
{
	BulletCount += Count;
}

uint8 Inventory::GetBulletCount() const
{
	return BulletCount;
}


