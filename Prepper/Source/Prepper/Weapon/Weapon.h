#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "Prepper/Item/AInteractable.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	
	EWS_MAX UMETA(DisplayName = "Default MAX")
};

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	EFT_MAX UMETA(DisplayName = "DefaultMAX")
};
UCLASS()
class PREPPER_API AWeapon : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Interaction(APlayerCharacter* Target) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;

	void SetHUDAmmo();
	virtual void Fire(const FVector& HitTarget);
	void Dropped();

	UPROPERTY(EditAnywhere, Category = Crosshiar)
	class UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere, Category = Crosshiar)
	UTexture2D* CrosshairLeft;
	UPROPERTY(EditAnywhere, Category = Crosshiar)
	UTexture2D* CrosshairRight;
	UPROPERTY(EditAnywhere, Category = Crosshiar)
	UTexture2D* CrosshairTop;
	UPROPERTY(EditAnywhere, Category = Crosshiar)
	UTexture2D* CrosshairBottom;

	// Zoom FOV
	UPROPERTY(EditAnywhere)
	float ZoomFOV = 30.f;
	
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = false;

	void AddAmmo(int32 AmmoToAdd);


	// 산탄
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;
	
	FVector TraceEndWithScatter(const FVector& HitTarget);

	UPROPERTY()
	class USoundCue* EquipSound;

	/*
	 * Custom Depth 아이템 윤곽선 효과
	 * 
	 */
	void EnableCustomDepth(bool bEnable);

	UPROPERTY(EditAnywhere)
	EFireType FireType;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;
	
	float TargetDistance;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(EditAnywhere,Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);

	int32 Sequence = 0;

	UPROPERTY(EditAnywhere)
	int32 MagCapacity;
	
	UPROPERTY(EditAnywhere)
	bool bAutoReload = false;
	void SpendRound();

	

	UPROPERTY()
	class APlayerCharacter* PlayerOwnerCharacter;
	UPROPERTY()
	class APrepperPlayerController* PlayerOwnerController;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
public:
	void SetWeaponState(EWeaponState State);
	bool IsAmmoEmpty();
	
	FORCEINLINE USphereComponent* GetAreaSphere()		const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV()					const { return ZoomFOV; }
	FORCEINLINE float GetZoomedInterpSpeed()			const { return ZoomInterpSpeed; }
	FORCEINLINE EWeaponType GetWeaponType()				const { return WeaponType; }
	FORCEINLINE int32 GetAmmo()							const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity()					const { return MagCapacity; }
	FORCEINLINE bool GetAutoReload()					const { return bAutoReload; }
};


