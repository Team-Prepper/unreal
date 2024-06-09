#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Prepper/Enums/TurningInPlace.h"
#include "Prepper/Enums/CombatState.h"
#include "Prepper/Interfaces/InteractWithCrosshairInterface.h"
#include "Prepper/Interfaces/Controllable.h"
#include "Prepper/Interfaces/PlayerAbility.h"
#include "Prepper/Interfaces/Weapon.h"
#include "Prepper/Item/MapInventory.h"
#include "PlayerCharacter.generated.h"

class UInputAction;

struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	EPMS_Seat UMETA(DisplayName = "Seat"),
	EPMS_Aim UMETA(DisplayName = "Aim"),
	EPMS_Sprint UMETA(DisplayName = "Sprint"),
	EPMS_Idle UMETA(DisplayName = "Idle"),
    
	EPMS_MAX UMETA(DisplayName = "Default Max")
};

UCLASS()
class PREPPER_API APlayerCharacter : public ABaseCharacter,
									public IInteractWithCrosshairInterface,
									public IControllable, public IPlayerAbility
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	//combatcomponent 에서 사용
	void PlayFireMontage(bool bAiming); 
	void PlayReloadMontage(const FName& SectionName);
	void PlaySwapMontage();
	
	virtual void Elim() override;
	virtual void MulticastElim() override;


	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
	void EquipBackpack(class AItemBackpack* BackpackToEquip);

	virtual void AttachActorAtSocket(const FName& SocketName, AActor* TargetActor) override;
	
	bool bFinishedSwapping = false;
	
	UPROPERTY(Replicated)
	bool bDisableGamePlay = false;

	
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
public:
	UFUNCTION(BlueprintCallable)
	virtual UCameraComponent* GetFollowCamera() override;
protected:
	/* 행동관련 */
	virtual void Jump() override;
	
	// 자연스러운 회전 - 멀티플레이 proxies
	void SimProxiesTurn();
	float TimeSinceLastMovementReplication;
	
	/* 입력 관련 */
private:	
	// Aim 연산
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float AimMovementSpeed = 400.f;

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed(AWeaponActor* Weapon);

	bool bIsSprint = false;
	
protected:
	virtual void ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser) override;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCombatComponent* Combat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UInteractionComponent* InteractionComponent;

	/* 상태 이상 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UStatusEffectComponent* StatusEffect;

	
	void SetPlayerEqiupmentHiddenInGame(bool visible);
	
	/* For Animation */
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* MeleeWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;

	/* Player Movement State */
	bool bBeforeSeat;
	
	UPROPERTY(Replicated)
	EPlayerMovementState PlayerMovementState;

	UPROPERTY(VisibleAnywhere, Category = "Player Noise")
	UPawnNoiseEmitterComponent* PawnNoiseEmitter; // 노이즈 발생 컴포넌트
public:
	void SetPlayerMovementState(const EPlayerMovementState State);

	float CoefficientMovementSpeed = 1;
	
private:
	void ConvertPlayerMovementState();
	UFUNCTION(Server, Reliable)
	void ServerConvertPlayerMovementState(const EPlayerMovementState State);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastConvertPlayerMovementState(const EPlayerMovementState State);
	
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
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedBackpack)
	AItemBackpack* EquippedBackpack;

	UFUNCTION()
	void OnRep_EquippedBackpack();

public:
	UPROPERTY()
	class UMapInventory* Inven;

	virtual void OpenCraftingTable() override;
	virtual void AddItem(FString ItemCode) override;
	virtual void UseQuickSlotItem(int Idx) override;
	virtual void EquipWeapon(AWeaponActor* Weapon) override;
	
	virtual void Heal(int Amount) override;
	virtual void Eat(int Amount) override;
	virtual void Drink(int Amount) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddItem(const FString& ItemCode);
	
	bool IsWeaponEquipped();
	bool IsAiming();
	bool IsLocallyReloading();
	AWeaponActor* GetEquippedWeapon();
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	FORCEINLINE EPlayerMovementState GetPlayerMovementState() const { return PlayerMovementState; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return Combat; }
	FORCEINLINE UStatusEffectComponent* GetStatusEffectComponent() const { return StatusEffect; }
	FORCEINLINE bool GetDisableGamePlay() const { return bDisableGamePlay; }
	
};
