#include "CharacterControlMapper.h"

#include "Prepper/Character/BaseCharacter.h"
#include "Prepper/Character/PlayerCharacter.h"

void UCharacterControlMapper::ToggleControlWidget(bool Toggle)
{
	/*
	if (CharacterOverlay == nullptr)
	{
		CharacterOverlay = CreateWidget<UUserWidget>(TargetCharacter->Controller, CharacterOverlayClass);
	}

	const ESlateVisibility Visibility =
		Toggle ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	CharacterOverlay->SetVisibility(Visibility);
*/
}

void UCharacterControlMapper::Move(const FInputActionValue& Value)
{
	TargetCharacter->Move(Value);
}

void UCharacterControlMapper::Look(const FInputActionValue& Value)
{
	TargetCharacter->Look(Value);
}

void UCharacterControlMapper::ShiftPressed()
{
	TargetCharacter->SetMovementState(EMovementState::EMS_Sprint);
}

void UCharacterControlMapper::ShiftReleased()
{
	TargetCharacter->SetMovementState(EMovementState::EMS_Idle);
}

void UCharacterControlMapper::SpacePressed()
{
	TargetCharacter->Jump();
}

void UCharacterControlMapper::SpaceReleased()
{
	TargetCharacter->StopJumping();
}

void UCharacterControlMapper::EPressed()
{
	TargetCharacter->EPressed();
}

void UCharacterControlMapper::RPressed()
{
	TargetCharacter->RPressed();
}

void UCharacterControlMapper::ControlPressed()
{
	TargetCharacter->ControlPressed();
}

void UCharacterControlMapper::MouseLeftPressed()
{
	TargetCharacter->AttackTrigger(true);
}

void UCharacterControlMapper::MouseLeftReleased()
{
	TargetCharacter->AttackTrigger(false);
}

void UCharacterControlMapper::MouseRightPressed()
{
	TargetCharacter->AimTrigger(true);
}

void UCharacterControlMapper::MouseRightReleased()
{
	TargetCharacter->AimTrigger(false);
}

UCameraComponent* UCharacterControlMapper::GetFollowCamera()
{
	return TargetCharacter->GetFollowCamera();
}
