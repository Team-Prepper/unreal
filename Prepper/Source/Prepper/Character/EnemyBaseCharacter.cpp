#include "EnemyBaseCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "Prepper/Prepper.h"
#include "Prepper/Weapon/WeaponActor.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(100.f);
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyController = Cast<AAIController>(GetController());
	SpawnWeaponActor();
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBaseCharacter::PawnSeen);
		PawnSensing->OnHearNoise.AddDynamic(this, &AEnemyBaseCharacter::PawnHearn);
	}
}

void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bElimed) return;
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
	if(bElimed) return;
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBaseCharacter::PatrolTimerFinished, WaitTime);
	}
}

void AEnemyBaseCharacter::CheckCombatTarget()
{
	if(bElimed) return;
	/* ees가 attack이거나 chase 일때만 실행 */

	// 공격사거리 안에서 공격이 아닐떄 -> 공격!
	if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp, Warning, TEXT("CODE : zombie Attack"));
		PawnAttack();
	}
	// 공격사거리보다는 멀고 탐지사거리보다는 가까운데 
	// 감지 사거리 내에서 순찰중일때
	else if (!InTargetRange(CombatTarget, AttackRadius) && InTargetRange(CombatTarget, CombatRadius) && EnemyState < EEnemyState::EES_Chasing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Found Target -> chasing"));
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		MoveToTarget(CombatTarget);
	}
	else if (!InTargetRange(CombatTarget, CombatRadius))
	{
		UE_LOG(LogTemp, Warning, TEXT("Lost Target"));
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(PatrolTarget);
	}
	
	
}

void AEnemyBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBaseCharacter, EquippedWeapon);
}

void AEnemyBaseCharacter::ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(Damage, InstigatorController, DamageCauser);
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
	if(bElimed) return;
	if (EnemyController == nullptr || Target == nullptr) return;
	EnemyController->MoveToActor(Target, 15.f);
}

void AEnemyBaseCharacter::MoveToLocation(FVector& Location)
{
	if(bElimed) return;
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
	if(bElimed) return;
	if (!SeenPawn->ActorHasTag(FName("PlayerCharacter"))) return; 
	// 엑터의 태그가 플레이어일때만
	UE_LOG(LogTemp, Warning, TEXT("zombie SEE -> chasing"));
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	CombatTarget = SeenPawn;
	
	if (EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Chasing;
		FVector TargetLocation = CombatTarget->GetActorLocation(); // 플레이어의 위치를 복사하여 전달
		MoveToLocation(TargetLocation); // 플레이어 위치로 이동
	}
	
}

void AEnemyBaseCharacter::PawnHearn(APawn *HearnPawn, const FVector &Location, float Volume)
{
	if(bElimed) return;
	UE_LOG(LogTemp, Display, TEXT("CODE : zombie HEAR"));
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

void AEnemyBaseCharacter::Elim()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	}
	Super::Elim();
}

void AEnemyBaseCharacter::PawnAttack()
{
	if(bElimed) return;
	if(!bCanAttack) return;
	UE_LOG(LogTemp, Warning, TEXT("zombie Attack"));
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyBaseCharacter::AttackCoolDown, AttackCoolTime);
	PlayAttackMontage();
	
	if(EquippedWeapon)
	{
		HitTargets = EquippedWeapon->GetTarget(HitTarget);
		EquippedWeapon->Fire(HitTargets);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Weapon"));
	}
}

void AEnemyBaseCharacter::PlayAttackMontage()
{
	MulticastPlayAttackMontage();
}

void AEnemyBaseCharacter::MulticastPlayAttackMontage_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}

void AEnemyBaseCharacter::SpawnWeaponActor()
{
	if(!HasAuthority()) return;
	if (WeaponActorClass == nullptr) return;
	
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // Setting the owner
	EquippedWeapon = World->SpawnActor<AWeaponActor>(WeaponActorClass, Location, Rotation, SpawnParams);

	if (!EquippedWeapon) return;
	UE_LOG(LogTemp, Warning, TEXT("Spawned Weapon Actor: %s"), *EquippedWeapon->GetName());

	EquippedWeapon->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	EquippedWeapon->SetOwner(this);
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorAtSocket(FName("MeleeWeaponSocket"),EquippedWeapon);
}

void AEnemyBaseCharacter::AttackCoolDown()
{
	bCanAttack = true;
	EnemyState = EEnemyState::EES_Patrolling;
	CheckCombatTarget();
}
