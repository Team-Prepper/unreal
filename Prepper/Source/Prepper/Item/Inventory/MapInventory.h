#pragma once

#define MAX_QUICK_SLOT 5

#include <set>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/ItemManager.h"
#include "Prepper/Interfaces/Inventory.h"
#include "Prepper/_Base/ObserverPattern/Subject.h"
#include "Net/UnrealNetwork.h" // 네트워크 관련 헤더 추가
#include "MapInventory.generated.h"

// 아이템 코드와 수량을 저장할 구조체 이름을 FItemConvertData로 변경
USTRUCT(BlueprintType)
struct FItemConvertData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ItemCode;

	UPROPERTY()
	uint8 Count;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PREPPER_API UMapInventory : public UActorComponent, public IInventory, public ISubject<TArray<FItemConvertData>>
{
	GENERATED_BODY()
private:
	IPlayerAbility* Owner;

	uint8 BulletCount;

	// 복제 가능한 TArray로 ItemUnits를 대체
	UPROPERTY(ReplicatedUsing = OnRep_ItemUnits)
	TArray<FItemConvertData> ReplicatedItemUnits;

	TMap<FString, uint8> ItemUnits; // 실제 아이템 저장용

	FString QuickSlotItem[MAX_QUICK_SLOT];

	UPROPERTY(EditAnywhere)
	uint8 InventorySize = 16;

private:
	std::pmr::set<IObserver<TArray<FItemConvertData>>*> Observers;

public:
	UMapInventory();
	virtual bool TryAddItem(const FString& ItemCode) override;
	virtual bool TryUseItem(const FString& ItemCode) override;
	virtual bool TryDiscardItem(const FString& ItemCode) override;
	virtual int TryGetItemCount(const FString& ItemCode) override;
	virtual bool CheckOwnItem(const FString& ItemCode) override;

	virtual void QuickSlotAdd(const FString& ItemCode, const int Idx) override;
	virtual void UseItemAtQuickSlot(const int Idx) override;

	virtual TArray<InventoryItem> GetIter() const override;

	void AddBullet(uint8 Count);
	uint8 GetBulletCount() const;

	virtual void Attach(IObserver<TArray<FItemConvertData>>* Observer) override;
	virtual void Detach(IObserver<TArray<FItemConvertData>>* Observer) override;
	virtual void Notify() override;

	// 클라이언트에서 호출되는 RepNotify 함수
	UFUNCTION()
	void OnRep_ItemUnits();

	// 네트워크 복제를 위한 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// TMap을 TArray로 변환하는 함수
	void ConvertMapToArray();

	// TArray를 TMap으로 변환하는 함수
	void ConvertArrayToMap();
};
