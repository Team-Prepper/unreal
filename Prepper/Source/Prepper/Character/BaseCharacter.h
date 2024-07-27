#pragma once

#include <set>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Prepper/Interfaces/Damageable.h"
#include "Prepper/_Base/ObserverPattern/Subject.h"
#include "Prepper/_Base/Util/GaugeValue.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PREPPER_API ABaseCharacter : public ACharacter, public IDamageable, public ISubject<GaugeValue<float>>
{
	GENERATED_BODY()
// Actor
public:
	ABaseCharacter();
	// 네트워크 동기화 변수 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
	
// Observer Pattern
private:
	std::pmr::set<IObserver<GaugeValue<float>>*> Observers;
public:
	virtual void Attach(IObserver<GaugeValue<float>>* Observer) override;
	virtual void Detach(IObserver<GaugeValue<float>>* Observer) override;
	virtual void Notify() override;

// Character
public:
	void PlayAnim(UAnimMontage* Montage, const FName& SectionName = "") const;
	void AttachActorAtSocket(const FName& SocketName, AActor* TargetActor);
	
// Movement
protected:
	/* 기본 변수 */
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float WalkSpeed = 600;
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float SprintSpeed = 900;

protected:
	UPROPERTY()
	class APrepperPlayerController* PrepperPlayerController;
	
	UPROPERTY()
	class ADeathMatchPlayerState* DeathMatchPlayerState;

// Health
private:
	/* 데미지 처리 */
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;
protected:
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float CurrentHealth = 100.f;
public:
	
	UFUNCTION()
	virtual void ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser) override;

protected:
	virtual void PlayHitReactMontage();

private:
	UFUNCTION()
	virtual void OnRep_Health();

/* 사망 및 부활 처리 */
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;
	bool bElimed = false;

protected:
	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

public:
	virtual void Elim();
	FORCEINLINE bool IsElimed() const { return bElimed; }
private:
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastElim();

/* 사망 이펙트  */
private:
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

protected:
	void StartDissolve();
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
};
