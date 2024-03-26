// Copyright Epic Games, Inc. All Rights Reserved.


#include "CarController.h"
#include "CarPawn.h"
#include "PrepperUI.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void ACarController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// spawn the UI widget and add it to the viewport
	//VehicleUI = CreateWidget<UPrepperUI>(this, VehicleUIClass);

	//check(VehicleUI);

	//VehicleUI->AddToViewport();
}

void ACarController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
	}
}

void ACarController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	//VehiclePawn = CastChecked<APrepperPawn>(InPawn);
}
