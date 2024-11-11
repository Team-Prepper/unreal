#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DogPet.generated.h"

UCLASS()
class PREPPER_API ADogPet : public ACharacter
{
	GENERATED_BODY()

public:
	ADogPet();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

	// 플레이어를 따라가는 함수
	void FollowPlayer(float DeltaTime);

	// 플레이어 참조 변수
	ACharacter* PlayerCharacter;

	// 따라갈 거리와 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float FollowDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float MovementSpeed;
};