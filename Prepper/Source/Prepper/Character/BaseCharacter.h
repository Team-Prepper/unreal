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
private:
	std::pmr::set<IObserver<GaugeValue<float>>*> Observers;
public:
	// 네트워크 동기화 변수 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void AttachActorAtSocket(const FName& SocketName, AActor* TargetActor);
	
	virtual void Attach(IObserver<GaugeValue<float>>* Observer) override;
	virtual void Detach(IObserver<GaugeValue<float>>* Observer) override;
	virtual void Notify() override;

	ABaseCharacter();
protected:
	virtual void BeginPlay() override;

	/* 기본 변수 */
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float WalkSpeed = 600;
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float SprintSpeed = 900;

	UPROPERTY()
	class APrepperPlayerController* PrepperPlayerController;
	
	UPROPERTY()
	class ADeathMatchPlayerState* DeathMatchPlayerState;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float CurrentHealth = 100.f;
	
	UFUNCTION()
	virtual void OnRep_Health();

	/* 데미지 처리 */
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	
	UFUNCTION()
	virtual void ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser) override;

	virtual void PlayHitReactMontage();

	/* 사망 및 부활 처리 */
	bool bElimed = false;
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
	FORCEINLINE bool IsElimed() const { return bElimed; }
};
