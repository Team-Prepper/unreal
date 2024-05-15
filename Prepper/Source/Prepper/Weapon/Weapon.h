#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "Prepper/Item/AInteractableActor.h"
#include "Weapon.generated.h"


UCLASS()
class PREPPER_API AWeapon : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Interaction(APlayerCharacter* Target) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	
	void Dropped();

	/* Custom Depth 아이템 윤곽선 효과 */
	void EnableCustomDepth(bool bEnable);

	virtual void Fire(const TArray<FVector_NetQuantize>& HitTargets) PURE_VIRTUAL();

	virtual void SetHUDAmmo() PURE_VIRTUAL();

	virtual void GetCrosshair(UTexture2D* &Center, UTexture2D* &Left, UTexture2D* &Right, UTexture2D* &Top, UTexture2D* &Bottom);

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponSocketName = FName("RightHandSocket");

	UFUNCTION()
	FName AttachSocketName() { return WeaponSocketName; }
	
	virtual TArray<FVector_NetQuantize> GetTarget(FVector& HitTarget);
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	FName ReloadActionName = FName("AssaultRifle");
	
	UPROPERTY()
	USoundCue* EquipSound;
	
protected:
	virtual void BeginPlay() override;

	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

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
	APrepperPlayerController* PlayerOwnerController;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
public:
	void SetWeaponState(EWeaponState State);
	bool IsMeleeWeapon();
	FORCEINLINE USphereComponent* GetAreaSphere()		const { return AreaSphere; }
	FORCEINLINE UMeshComponent* GetWeaponMesh()			const { return WeaponMesh; }
	FORCEINLINE EWeaponType GetWeaponType()				const { return WeaponType; }
	

	UPROPERTY(VisibleAnywhere, Category = "Weapon Noise")
	UPawnNoiseEmitterComponent* PawnNoiseEmitter; // 노이즈 발생 컴포넌트
};


