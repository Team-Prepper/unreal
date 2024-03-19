#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Prepper/Enums/TurningInPlace.h"
#include "Prepper/Interfaces/InteractWithCrosshairInterface.h"
#include "Prepper/Item/Inventory.h"
#include "PlayerCharacter.generated.h"

class UInputAction;

struct FInputActionValue;

UCLASS()
class PREPPER_API APlayerCharacter : public ACharacter, public IInteractWithCrosshairInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();
	
	virtual void OnRep_ReplicatedMovement() override;

	// DEATH MATCH
	void Elim();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
protected:
	virtual void BeginPlay() override;

	float WalkSpeed;
	float SprintSpeed;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void SprintButtonPressed();
	void SprintButtonReleased();
	
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	
	virtual void Jump() override;

	void PlayHitReactMontage();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

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
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingItem)
	class AInteractable* OverlappingItem;
	
	UFUNCTION()
	void OnRep_OverlappingItem(AInteractable* LastItem);

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed(AWeapon* Weapon);
	
	UFUNCTION(Server, Reliable)
	void ServerSprintButtonPressed();
	UFUNCTION(Server, Reliable)
	void ServerSprintButtonReleased();
	
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	/*
	 * For Animation 
	 */

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	/*
	 * For Crouch Cam
	 */

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
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	Inventory Inven;

	class APrepperPlayerController* PrepperPlayerController;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float CurrentHealth = 100.f;

	UFUNCTION()
	void OnRep_Health();

	bool bElimmed = false;

	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();

public:
	void SetOverlappingItem(AInteractable* InteractableItem);
	void EquipWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	void AddItem(FString& ItemCode);
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;

public:
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimed() const { return bElimmed; }
};
