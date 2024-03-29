#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PREPPER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();
	// 네트워크 동기화 변수 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	/* 기본 변수 */
	float WalkSpeed;
	float SprintSpeed;

	UPROPERTY()
	class APrepperPlayerController* PrepperPlayerController;
	
	UPROPERTY()
	class ADeathMatchPlayerState* DeathMatchPlayerState;
	

	/* 체력 관련 */
protected:
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float CurrentHealth = 100.f;

	
	UFUNCTION()
	void OnRep_Health();

	/* 데미지 처리 */
protected:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;
	
	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	virtual void UpdateHUDHealth();

	virtual void PlayHitReactMontage();

	/* 사망 및 부활 처리 */
protected:
	bool bElimmed = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;
	void PlayElimMontage();

public:
	virtual void Elim();
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastElim();

	/* 사망 이펙트  */
protected:
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;
	
	// DissolveMaterialInstance로 부터 동적 생성
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// 블루 프린트에 세팅
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

public:
	FORCEINLINE bool IsElimed() const { return bElimmed; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};
