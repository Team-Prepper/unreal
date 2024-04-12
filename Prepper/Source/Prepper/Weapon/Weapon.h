#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "Prepper/Item/AInteractable.h"
#include "Weapon.generated.h"


UCLASS()
class PREPPER_API AWeapon : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Interaction(APlayerCharacter* Target) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	
	void Dropped();

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;
	
	UPROPERTY()
	class USoundCue* EquipSound;

	/* Custom Depth 아이템 윤곽선 효과 */
	void EnableCustomDepth(bool bEnable);

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere,Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();
	
	UPROPERTY()
	class APlayerCharacter* PlayerOwnerCharacter;
	UPROPERTY()
	class APrepperPlayerController* PlayerOwnerController;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
public:
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere()		const { return AreaSphere; }
	FORCEINLINE EWeaponType GetWeaponType()				const { return WeaponType; }
	FORCEINLINE UMeshComponent* GetWeaponMesh()			const { return WeaponMesh; }
};


