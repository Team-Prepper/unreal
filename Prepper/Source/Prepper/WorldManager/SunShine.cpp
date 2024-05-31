#include "SunShine.h"

#include "Components/DirectionalLightComponent.h"

ASunShine::ASunShine()
{
	PrimaryActorTick.bCanEverTick = true;
	DayLength = 60.0f;

	DirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLightComponent"));
	RootComponent = DirectionalLightComponent;
}

void ASunShine::BeginPlay()
{
	Super::BeginPlay();
}

void ASunShine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLightRotation(DeltaTime);
}

void ASunShine::UpdateLightRotation(float DeltaTime)
{
	if (DirectionalLightComponent)
	{
		FVector RotationAxis = FVector(1.0f, 0.5f, 0.0f).GetSafeNormal();

		// Set the rotation speed (radians per second)
		float RotationSpeed = 2.0f * PI / DayLength;

		// Calculate the rotation angle based on the rotation speed and DeltaTime
		float RotationAngle = RotationSpeed * DeltaTime;

		// Calculate the rotation quaternion
		FQuat RotationQuat = FQuat(RotationAxis, RotationAngle);

		// Apply the rotation to the light's direction vector
		FVector NewDirection = RotationQuat.RotateVector(DirectionalLightComponent->GetDirection());

		// Set the new direction of the directional light
		DirectionalLightComponent->SetWorldRotation(NewDirection.Rotation());
	}
}

