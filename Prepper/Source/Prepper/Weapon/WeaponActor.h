#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "Prepper/Interfaces/Weapon.h"
#include "Prepper/Interfaces/WeaponHandler.h"
#include "Prepper/Object/InteractableActor.h"
#include "Sound/SoundCue.h"
#include "WeaponActor.generated.h"


UCLASS()
class PREPPER_API AWeaponActor : public AInteractableActor, public IWeapon
{
	GENERATED_BODY()
private:
	/* Custom Depth 아이템 윤곽선 효과 */
	void EnableCustomDepth(bool bEnable);
	
public:	
	AWeaponActor();
	
	virtual EWeaponType GetWeaponType() override { return WeaponType; };
	virtual void SetWeaponState(EWeaponState State) override;
	virtual void Interaction(APlayerCharacter* Target) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	virtual void SetWeaponHandler(IWeaponHandler* NewOwner) override;
	virtual int GetLeftAmmo() override { return -1; }

	virtual TArray<FVector_NetQuantize> GetTarget(FVector& HitTarget) override;

	virtual void GetCrosshair(
		float DeltaTime, bool bIsAiming, 
		UTexture2D* &Center,
		UTexture2D* &Left,
		UTexture2D* &Right,
		UTexture2D* &Top,
		UTexture2D* &Bottom,
		float &Spread) override;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponSocketName = FName("RightHandSocket");
	UPROPERTY(EditAnywhere, Category = Combat)
	FName HolsteredWeaponSocketName = FName("HolsteredWeaponSocket");
	UPROPERTY(EditAnywhere, Category = Combat)
	FName ReloadActionName = FName("AssaultRifle");

	virtual FName AttachSocketName() override { return WeaponSocketName; };
	virtual FName GetReloadActionName() override { return ReloadActionName; };
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	virtual float GetFireDelay() override { return FireDelay; };
	
	UPROPERTY()
	USoundCue* EquipSound;
	
	void PlayEquipWeaponSound();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

	IWeaponHandler* GetWeaponHandler();

	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);
	
	UPROPERTY(Replicated, EditAnywhere)
	bool bUseServerSideRewind = false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UStaticMeshComponent* StaticWeaponMesh;

	UPROPERTY(EditAnywhere,Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();
	
	UPROPERTY()
	APlayerCharacter* PlayerOwnerCharacter;
	UPROPERTY()
	class APrepperPlayerController* PlayerOwnerController;
	
	IWeaponHandler* WeaponHandler;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

private:
	void WeaponPhysicsActive(bool active);
	
public:
	FORCEINLINE USphereComponent* GetAreaSphere()		const { return AreaSphere; }
	FORCEINLINE UMeshComponent* GetWeaponMesh()			const { return WeaponMesh; }
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Noise")
	UPawnNoiseEmitterComponent* PawnNoiseEmitter; // 노이즈 발생 컴포넌트
};