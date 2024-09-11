#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Component/CharacterComponent.h"
#include "Prepper/Component/CustomCameraComponent.h"
#include "Prepper/Enums/TurningInPlace.h"
#include "Prepper/Enums/CombatState.h"
#include "Prepper/Interfaces/InteractWithCrosshairInterface.h"
#include "Prepper/Interfaces/Controllable.h"
#include "Prepper/Interfaces/PlayerAbility.h"
#include "Prepper/Item/MapInventory.h"
#include "PlayerCharacter.generated.h"

class UFlexibleSpringArmComponent;
class UInputAction;

struct FInputActionValue;
UCLASS()
class PREPPER_API APlayerCharacter : public ABaseCharacter,
									 public IInteractWithCrosshairInterface,
									 public IControllable, public IPlayerAbility
{
	GENERATED_BODY()

// Actor
public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	
// BaseCharacter
public:
	UPROPERTY(Replicated)
	bool bDisableGamePlay = false;
public:
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;
	
	virtual void Elim() override;
	virtual void MulticastElim() override;
	
	virtual void ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
protected:
	virtual void PlayHitReactMontage() override;
	
	/* 행동관련 */
	virtual void Jump() override;
// IPlayerAbility
private:
	UPROPERTY()
	UMapInventory* Inventory;
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedBackpack)
	AItemBackpack* EquippedBackpack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UStatusEffectComponent* StatusEffect;

	UPROPERTY()
	class APrepperHUD* PrepperHUD;
	
	std::pmr::set<IObserver<UMapInventory>*> InventoryObservers;

	
public:

	virtual void AddItem(const FString& ItemCode) override;
	virtual void UseQuickSlotItem(int Idx) override;
	virtual void EquipWeapon(AWeaponActor* Weapon) override;
	virtual void EquipBackpack(class AItemBackpack* BackpackToEquip) override;
	
	virtual void Heal(float Amount) override;
	virtual void Eat(float Amount) override;
	virtual void Drink(float Amount) override;

private:
	UFUNCTION()
	void OnRep_EquippedBackpack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddItem(const FString& ItemCode);
	
// IControllable
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCombatComponent* Combat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UInteractionComponent* InteractionComponent;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UFlexibleSpringArmComponent* FlexibleCameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCustomCameraComponent* FollowCamera;

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	float TimeSinceLastMovementReplication;
	
public:
	// 서버에서만 실행되는 함수들 
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

	virtual void ToggleInventory() override;

	virtual void SeatToggle(const bool Seat) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastToggleInventory();
	
	UFUNCTION(BlueprintCallable)
	virtual UCustomCameraComponent* GetFollowCamera() override;
	
private:	
	// Aim 연산
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
	// 자연스러운 회전 - 멀티플레이 proxies
	void SimProxiesTurn();

	void RotateInPlace(float DeltaTime);

// ETC
protected:
	// init 되었는지 확인하고 init함 _ DeathMatch
	void PollInit();
	virtual void OnRep_ReplicatedMovement() override;

private:
	UPROPERTY(EditAnywhere, Category = Skin)
	UStaticMeshComponent* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* OverheadWidget;

	void SetPlayerEquipmentHiddenInGame(bool visible);
	void SetEquipmentHidden(AActor* Target, bool visible);
	
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

	UPROPERTY(VisibleAnywhere, Category = "Player Noise")
	UPawnNoiseEmitterComponent* PawnNoiseEmitter; // 노이즈 발생 컴포넌트
public:
	
private:
	void HideCamIfCharacterClose();
	UPROPERTY(EditAnywhere)
	float CamThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	
	float CalculateSpeed();

public:
	
	bool IsWeaponEquipped();
	bool IsAiming();
	bool IsLocallyReloading();
	
	AWeaponActor* GetEquippedWeapon();
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE UBaseCombatComponent* GetCombatComponent() const { return CombatComp; }
	FORCEINLINE UStatusEffectComponent* GetStatusEffectComponent() const { return StatusEffect; }
	FORCEINLINE bool GetDisableGamePlay() const { return bDisableGamePlay; }
	
};
