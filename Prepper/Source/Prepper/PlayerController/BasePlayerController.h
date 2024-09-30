#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Prepper/ControlMapper/ControlMapper.h"
#include "BasePlayerController.generated.h"

class UCharacterOverlay;
class UCompass;
class APrepperHUD;
class APlayerCharacter;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

UCLASS()
class PREPPER_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;
	virtual void OnPossess();
	virtual void PlayerTick(float DeltaTime) override;
	void ResetPlayer();
protected:
	virtual void BeginPlay() override;
	virtual void PossessPlayerCharacter();

	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UCharacterOverlay> CharacterOverlayClass;
	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;

	UPROPERTY(EditAnywhere, Category = "Player HUD")
	TSubclassOf<UCompass> CompassHUDClass;
	UPROPERTY()
	UCompass* Compass;
	
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	APrepperHUD* PrepperHUD;
	APrepperHUD* GetPrepperHUD();
	
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

	IControlMapper* TargetControlMapper = nullptr;
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

	virtual void SetInput(UEnhancedInputComponent* Input);
	
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

	UFUNCTION(Server, Reliable)
	void ServerInteractionPressed();

};
