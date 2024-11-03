#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Prepper/Interfaces/IDetectable.h"
#include "AAIHelper.generated.h"

class APlayerCharacter;

UCLASS()
class PREPPER_API UAIHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	UAIHelper();

	UPROPERTY(EditAnywhere, Category = "Detection")
	float DetectionRadius = 500.0f;

	// 감지 주기 (초 단위)
	UPROPERTY(EditAnywhere, Category = "Detection", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float DetectionInterval = 1.0f;

protected:
	virtual void BeginPlay() override;
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	void DetectNearbyItems();
    
	UPROPERTY()
	APlayerCharacter* OwningPlayer;

	// 감지된 아이템의 정보를 저장할 구조체
	struct FDetectedItemInfo
	{
		float LastDetectedTime;
		float Distance;

		FDetectedItemInfo(float InTime = 0.0f, float InDistance = 0.0f)
			: LastDetectedTime(InTime)
			, Distance(InDistance)
		{}
	};

	// Key: Actor 포인터, Value: 감지된 아이템 정보
	TMap<AActor*, FDetectedItemInfo> DetectedItems;

	void OnItemEnterRange(AActor* Item, IDetectable* DetectableItem, float Distance);
	void UpdateItemDistance(AActor* Item, float NewDistance);
	// 아이템이 범위를 벗어났을 때 호출
	void OnItemLeaveRange(AActor* Item);

	// 타이머 핸들
	FTimerHandle DetectionTimerHandle;
};
