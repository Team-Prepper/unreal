#pragma once

#include <set>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ElimEvent/ElimDissolveComponent.h"
#include "Prepper/Interfaces/Damageable.h"
#include "Prepper/_Base/ObserverPattern/Subject.h"
#include "Prepper/_Base/Util/GaugeValue.h"
#include "BaseCharacter.generated.h"


UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	EPMS_Seat UMETA(DisplayName = "Seat"),
	EPMS_Aim UMETA(DisplayName = "Aim"),
	EPMS_Sprint UMETA(DisplayName = "Sprint"),
	EPMS_Idle UMETA(DisplayName = "Idle"),
    
	EPMS_MAX UMETA(DisplayName = "Default Max")
};

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
	
private:
	std::pmr::set<IObserver<GaugeValue<float>>*> Observers;
protected:
	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	
	/* 기본 변수 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float CurrentHealth = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float WalkSpeed = 600;
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float SprintSpeed = 900;
	UPROPERTY(EditAnywhere, Category = "Player Movement Speed")
	float AimMovementSpeed = 400.f;
	
	/* 데미지 처리 */
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UElimDissolveComponent* ElimEvent;
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;
	
public:
	virtual void Attach(IObserver<GaugeValue<float>>* Observer) override;
	virtual void Detach(IObserver<GaugeValue<float>>* Observer) override;
	virtual void Notify() override;

	virtual void Elim();
	FORCEINLINE bool IsElimed() const { return CurrentHealth <= 0; }
	
	void PlayAnim(UAnimMontage* Montage, const FName& SectionName = "") const;
	void AttachActorAtSocket(const FName& SocketName, AActor* TargetActor);
	
	UFUNCTION()
	virtual void ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser) override;
	
	// Movement
public:
	float CoefficientMovementSpeed = 1;
	void SetPlayerMovementState(const EPlayerMovementState& State);
	
protected:
	UPROPERTY()
	class ADeathMatchPlayerState* DeathMatchPlayerState;
	
	UPROPERTY(Replicated)
	EPlayerMovementState PlayerMovementState;
	
	void ConvertPlayerMovementState(const EPlayerMovementState& State);
	UFUNCTION(Server, Reliable)
	void ServerConvertPlayerMovementState(const EPlayerMovementState& State);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastConvertPlayerMovementState(const EPlayerMovementState& State);

	virtual void SeatToggle(bool Seat);
	
	virtual void PlayHitReactMontage();
	UFUNCTION()
	virtual void OnRep_Health();

public:
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastElim();
};
