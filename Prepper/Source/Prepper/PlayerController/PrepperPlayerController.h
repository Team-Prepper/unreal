#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "PrepperPlayerController.generated.h"

class UInputAction;

UCLASS()
class PREPPER_API APrepperPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Value);
	void SetHUDCarriedAmmo(int32 Value);
	void SetHUDMatchCountDown(float CountDownTime);

	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
protected:
	virtual void BeginPlay() override;

	void SetHUDTime();
private:
	IControllable* TargetPlayer = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	class UInputMappingContext* PlayerMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* FireAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpButtonPressed();
	void JumpButtonReleased();
	void SprintButtonPressed();
	void SprintButtonReleased();
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();

	UPROPERTY()
	class APrepperHUD* PrepperHUD;
	
	/* 플레이 시간 카운터 */
	UPROPERTY(EditAnywhere)
	float MatchTime = 120.f;
	uint32 CountdownInt = 0;
	
};
