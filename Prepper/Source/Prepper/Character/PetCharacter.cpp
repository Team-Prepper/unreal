#include "PetCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

APetCharacter::APetCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = false;

	// Set default movement properties
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void APetCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive || !FollowTarget)
		return;

	FVector TargetLocation = FollowTarget->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - CurrentLocation).GetSafeNormal();
    
	float DistanceToTarget = FVector::Distance(CurrentLocation, TargetLocation);

	UE_LOG(LogTemp, Log, TEXT("거리: %f"), DistanceToTarget);
	if (DistanceToTarget > FollowDistance)
	{
		FVector DesiredLocation = TargetLocation - (DirectionToTarget * FollowDistance);
		AddMovementInput(DesiredLocation);
	}
}