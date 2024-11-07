#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PetCharacter.generated.h"

UCLASS()
class PREPPER_API APetCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APetCharacter();

	virtual void Tick(float DeltaTime) override;

	void SetFollowTarget(AActor* Target) { FollowTarget = Target; }
	void SetActive(bool bNewActive) { bIsActive = bNewActive; }
	bool IsActive() const { return bIsActive; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AActor* FollowTarget;

	UPROPERTY()
	bool bIsActive;

	UPROPERTY(EditAnywhere, Category = "Pet Properties")
	float FollowDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Pet Properties")
	float MovementSpeed = 300.0f;
};