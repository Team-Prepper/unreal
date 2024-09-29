#include "CarControlMapper.h"

#include "Blueprint/UserWidget.h"
#include "Prepper/Car/CarPawn.h"

void UCarControlMapper::ToggleControlWidget(bool Toggle, APlayerController* TargetController)
{
	if (!CharacterOverlayClass) return;
	if (CharacterOverlay == nullptr && Toggle)
	{
		CharacterOverlay = CreateWidget<UUserWidget>(TargetController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}

	if (!CharacterOverlay) return;
	
	const ESlateVisibility Visibility =
		Toggle ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	CharacterOverlay->SetVisibility(Visibility);
}

void UCarControlMapper::Connect(APlayerController* TargetController)
{
	ToggleControlWidget(true, TargetController);
}

void UCarControlMapper::Disconnect()
{
	ToggleControlWidget(false, nullptr);
}

void UCarControlMapper::Move(const FInputActionValue& Value)
{
	TargetCar->Move(Value);
}

void UCarControlMapper::Look(const FInputActionValue& Value)
{
	TargetCar->Look(Value);
}

void UCarControlMapper::ShiftPressed() {}
void UCarControlMapper::ShiftReleased() {}
void UCarControlMapper::SpacePressed() {}
void UCarControlMapper::SpaceReleased() {}

void UCarControlMapper::EPressed()
{
	TargetCar->EPressed();
}

void UCarControlMapper::RPressed(){}

void UCarControlMapper::ControlPressed()
{
	TargetCar->ControlPressed();
}

void UCarControlMapper::MouseLeftPressed(){}
void UCarControlMapper::MouseLeftReleased(){}
void UCarControlMapper::MouseRightPressed(){}
void UCarControlMapper::MouseRightReleased() {}

UCameraComponent* UCarControlMapper::GetFollowCamera()
{
	return TargetCar->GetFollowCamera();
}
