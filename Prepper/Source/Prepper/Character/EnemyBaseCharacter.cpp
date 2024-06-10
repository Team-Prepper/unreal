#include "EnemyBaseCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Prepper/Prepper.h"
#include "Prepper/HUD/GaugeBarComponent.h"
#include "TimerManager.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UGaugeBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	AttackCoolTime = 1.0f;
	AttackDamage = 10.0f;
	AttackTarget = nullptr;
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyController = Cast<AAIController>(GetController());
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBaseCharacter::PawnSeen);
		PawnSensing->OnHearNoise.AddDynamic(this, &AEnemyBaseCharacter::PawnHearn);
	}
}

void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBaseCharacter::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBaseCharacter::PatrolTimerFinished, WaitTime);
	}
}

void AEnemyBaseCharacter::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState == EEnemyState::EES_Attacking)
	{
		StopAttack();
	}
	else if (!InTargetRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		StartAttack(CombatTarget);
		
	}
}


void AEnemyBaseCharacter::StartAttack(AActor* Target)
{
	EnemyState = EEnemyState::EES_Attacking;
	AttackTarget = Target;
	Attack(); // 첫 타 먼저 치고 시작
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyBaseCharacter::Attack, AttackCoolTime, true);
}
void AEnemyBaseCharacter::StopAttack()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	AttackTarget = nullptr;
	EnemyState = EEnemyState::EES_Patrolling;
}
void AEnemyBaseCharacter::Attack()
{
	AController* InstigatorController = this->GetController(); // 좀비 공격은 항상 직접 공격
	UGameplayStatics::ApplyDamage(
		AttackTarget,
		AttackDamage,
		InstigatorController,
		this,
		UDamageType::StaticClass()
	);
}

void AEnemyBaseCharacter::ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(Damage, InstigatorController, DamageCauser);
}

void AEnemyBaseCharacter::UpdateHUDHealth()
{
	if (HealthBarWidget)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		HealthBarWidget->SetVisibility(true);
		HealthBarWidget->SetGaugePercent(HealthPercent);
	}
	
}

void AEnemyBaseCharacter::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

bool AEnemyBaseCharacter::InTargetRange(AActor* Target, float Radius)
{
	if (Target == nullptr) return false;
	const float DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemyBaseCharacter::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	EnemyController->MoveToActor(Target, 15.f);
}

void AEnemyBaseCharacter::MoveToLocation(FVector& Location)
{
	if (EnemyController == nullptr || Location == FVector::ZeroVector) return;
	EnemyController->MoveToLocation(Location, 1.5f);
}


AActor* AEnemyBaseCharacter::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemyBaseCharacter::PawnSeen(APawn* SeenPawn)
{
	UE_LOG(LogTemp, Display, TEXT("zombie SEE"));
	if (SeenPawn->ActorHasTag(FName("PlayerCharacter"))) // 엑터의 태그가 플레이어일때만
	{	
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;
		
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			FVector TargetLocation = CombatTarget->GetActorLocation(); // 플레이어의 위치를 복사하여 전달
			MoveToLocation(TargetLocation); // 플레이어 위치로 이동
		}
	}
}

void AEnemyBaseCharacter::PawnHearn(APawn *HearnPawn, const FVector &Location, float Volume)
{
	UE_LOG(LogTemp, Display, TEXT("zombie HEAR"));
	if (EnemyState == EEnemyState::EES_Chasing) return;
	
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	CombatTarget = HearnPawn;
	
	if (EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Chasing;
		FVector TargetLocation = CombatTarget->GetActorLocation(); // 플레이어의 위치를 복사하여 전달
		MoveToLocation(TargetLocation); // 플레이어 위치로 이동
	}
}
