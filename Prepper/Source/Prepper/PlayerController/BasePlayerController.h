#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "BasePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

class UInputAction;

UCLASS()
class PREPPER_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void PollInit() PURE_VIRTUAL();
	void SetPossessPawn();
	void ResetPlayer();
protected:
	virtual void BeginPlay() override;

	virtual void PossessPawn();
	
	FTimerHandle TimerHandle;
	
	void PossessNewPawn();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPossessNewPawn();

	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	class APrepperHUD* PrepperHUD;
public:
	/* NetWork */
	FHighPingDelegate HighPingDelegate;

	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);

	/* HIGH PING WARNING */
	float HighPingRunningTime = 0.f;
	float PingAnimationRunningTime = 5.f;
	
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 10.f;
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
	
	void CheckPing(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void HighPingWarningBP();

	UFUNCTION(BlueprintImplementableEvent)
	void StopHighPingWarningBP();

	/* Input Component */
public:
	virtual void SetupInputComponent() override;

	TScriptInterface<IControllable> TargetControllerable = nullptr;
protected:
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* OpenInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* Button1;
	
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
	void OpenInventoryPressed();

	void QuickSlot1Use();

	UFUNCTION(Server, Reliable)
	void ServerInteractionPressed();

	UFUNCTION(Server, Reliable)
	void ServerToggleInventory();

};
