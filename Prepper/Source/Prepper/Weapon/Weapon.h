#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prepper/Item/Interactable.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	
	EWS_MAX UMETA(DisplayName = "Default MAX")
};

UCLASS()
class PREPPER_API AWeapon : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Interaction(APlayerCharacter* Target) override;
	virtual void ShowPickUpWidget(bool bShowWidget) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
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
	bool bAutomatic = true;
	
protected:
	virtual void BeginPlay() override;

	float TargetDistance;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickUpWidget;

	UPROPERTY(EditAnywhere,Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	

public:
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV() const {return ZoomFOV; }
	FORCEINLINE float GetZoomedInterpSpeed() const {return ZoomInterpSpeed; }
};


