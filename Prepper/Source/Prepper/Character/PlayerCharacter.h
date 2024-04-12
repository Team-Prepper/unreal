#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Prepper/Enums/TurningInPlace.h"
#include "Prepper/Enums/CombatState.h"
#include "Prepper/Interfaces/InteractWithCrosshairInterface.h"
#include "Prepper/Interfaces/Controllable.h"
#include "Prepper/Item/Inventory.h"
#include "PlayerCharacter.generated.h"

class UInputAction;

struct FInputActionValue;

UCLASS()
class PREPPER_API APlayerCharacter : public ABaseCharacter, public IInteractWithCrosshairInterface, public IControllable
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	//combatcomponent 에서 사용
	void PlayFireMontage(bool bAiming); 
	void PlayReloadMontage();
	virtual void Elim() override;
	virtual void MulticastElim() override;

	UPROPERTY(Replicated)
	bool bDisableGamePlay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
private:
	void ElimTimerFinished();
	
protected:
	virtual void BeginPlay() override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;

	virtual void SpacePressed() override;
	virtual void SpaceReleased() override;

	virtual void ShiftPressed() override;
	virtual void ShiftReleased() override;

	virtual void EPressed() override;
	virtual void RPressed() override;
	virtual void ControlPressed() override;

	virtual void MouseLeftPressed() override;
	virtual void MouseLeftReleased() override;
	virtual void MouseRightPressed() override;
	virtual void MouseRightReleased() override;

	/* 행동관련 */
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	// 자연스러운 회전 - 멀티플레이 proxies
	void SimProxiesTurn();
	float TimeSinceLastMovementReplication;
	
	/* 입력 관련 */
private:	
	// Aim 연산
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed(AWeapon* Weapon);
	UFUNCTION(Server, Reliable)
	void ServerInteractionPressed();
	UFUNCTION(Server, Reliable)
	void ServerSprintButtonPressed();
	UFUNCTION(Server, Reliable)
	void ServerSprintButtonReleased();
	
protected:
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
	virtual void UpdateHUDHealth() override;
	
	// init 되었는지 확인하고 init함 _ DeathMatch
	void PollInit();

	virtual void PlayHitReactMontage() override;
	virtual void OnRep_ReplicatedMovement() override;

	void RotateInPlace(float DeltaTime);

	virtual void Destroyed() override;

private:
	/* 기본 캐릭터 구성 */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, Category = Skin)
	class UStaticMeshComponent* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* OverheadWidget;

	/* 아이템 획득 */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingItem)
	TScriptInterface<IIInteractable> OverlappingItem;
	UFUNCTION()
	void OnRep_OverlappingItem(TScriptInterface<IIInteractable> LastItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCombatComponent* Combat;
	
	/* For Animation */
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* MeleeWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;
	
	/* For Crouch Cam */
	UPROPERTY(EditAnywhere, Category = CrouchMovement)
	float CrouchCamOffset;
	UPROPERTY(EditAnywhere, Category = CrouchMovement)
	float DefaultCamOffset;
	UPROPERTY(EditAnywhere, Category = CrouchMovement)
	float CrouchCamArmLength;
	UPROPERTY(EditAnywhere, Category = CrouchMovement)
	float DefaultCamArmLength;
	float InterpSpeed;
	float TargetArmLength;
	FVector TargetSpringArmLocation;
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;

	void HideCamIfCharacterClose();
	UPROPERTY(EditAnywhere)
	float CamThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	
	float CalculateSpeed();

	Inventory Inven;

public:
	void SetOverlappingItem(AActor* InteractableItem);
	void EquipWeapon(AWeapon* Weapon);
	void DestroyInteractionItem(AInteractable* InteractableItem);
	UFUNCTION(Server, Reliable)
	void ServerDestroyInteractionItem(AInteractable* InteractableItem);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyInteractionItem(AInteractable* InteractableItem);
	bool IsWeaponEquipped();
	bool IsAiming();
	bool IsLocallyReloading();
	void AddItem(FString ItemCode);
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return Combat; }
	FORCEINLINE bool GetDisableGamePlay() const { return bDisableGamePlay; }
};
