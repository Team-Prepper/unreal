#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "PrepperPlayerController.generated.h"

class UInputAction;

UCLASS()
class PREPPER_API APrepperPlayerController : public APlayerController
{
	/* Default */
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
	void PollInit();
	
	/* Set HUD*/
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Value);
	void SetHUDCarriedAmmo(int32 Value);
	void SetHUDMatchCountDown(float CountDownTime);
protected:
	void SetHUDTime();
private:
	UPROPERTY()
	class APrepperHUD* PrepperHUD;

	// play time counter (in game)
	UPROPERTY(EditAnywhere)
	float MatchTime = 120.f;
	uint32 CountdownInt = 0;
	
	/* Sync time client <-> server*/
public:
	virtual float GetServerTime(); // Synced with server world clock
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

protected:
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
	
	float ClientServerDelta = 0.f; // difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

	/* Match Mode */
public:
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;
	
	bool bInitCharacterOverlay = false;
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;
	
	void OnMatchStateSet(FName State);
	UFUNCTION()
	void OnRep_MatchState();

	// for init when end of progress
	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;
protected:
	void HandleMatchHasStarted();
	
	/* Input Component */
public:
	virtual void SetupInputComponent() override;

	IControllable* TargetPlayer = nullptr;
private:
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

	/* Binding */
public:
	void BindPlayerAction();
};
