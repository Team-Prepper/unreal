#include "PetCharacter.h"
#include "AIController.h"
#include "Component/AAIHelper.h"
#include "Component/AGPTAssistant.h"
#include "GameFramework/CharacterMovementComponent.h"

APetCharacter::APetCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = false;

	// Movement 설정 확인
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->bOrientRotationToMovement = true;
		MovementComp->MaxWalkSpeed = MovementSpeed;
		MovementComp->bConstrainToPlane = true;
		MovementComp->bSnapToPlaneAtStart = true;
	}

	// AI Helper 컴포넌트 추가
	AIHelper = CreateDefaultSubobject<UAIHelper>(TEXT("AIHelper"));
	GPTAssistant = CreateDefaultSubobject<UGPTAssistant>(TEXT("GPTAssistant"));
}

void APetCharacter::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
}

void APetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive || !FollowTarget)
		return;

	FVector TargetLocation = FollowTarget->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();
	float DistanceToTarget = FVector::Distance(CurrentLocation, TargetLocation);

	if (DistanceToTarget > FollowDistance)
	{
		// PlayerLocation 방향으로 Pet 이동
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + (Direction * 200.0f * DeltaTime); // 속도 조절

		// 새 위치를 설정
		SetActorLocation(NewLocation);
	}
}

