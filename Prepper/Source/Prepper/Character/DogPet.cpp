#include "DogPet.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ADogPet::ADogPet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 기본 값 설정
	FollowDistance = 200.0f;
	MovementSpeed = 300.0f;
}

void ADogPet::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐릭터 참조 가져오기
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ADogPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		FollowPlayer(DeltaTime);
	}
}

void ADogPet::FollowPlayer(float DeltaTime)
{
	FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
	float Distance = Direction.Size();

	if (Distance > FollowDistance)
	{
		Direction.Normalize();
		AddMovementInput(Direction, MovementSpeed * DeltaTime);
	}
}