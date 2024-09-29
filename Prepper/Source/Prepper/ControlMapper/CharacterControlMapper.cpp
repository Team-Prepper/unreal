#include "CharacterControlMapper.h"

#include "Blueprint/UserWidget.h"
#include "Prepper/Character/BaseCharacter.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Character/Component/StatusEffectComponent.h"
#include "Prepper/HUD/UI/CharacterOverlay/CharacterOverlay.h"

void UCharacterControlMapper::ToggleControlWidget(bool Toggle, APlayerController* TargetController)
{
	
}

void UCharacterControlMapper::Connect(APlayerController* TargetController)
{
	UE_LOG(LogTemp, Warning, TEXT("TEXT00"));
	
	if (!CharacterOverlayClass) return;
	
	UE_LOG(LogTemp, Warning, TEXT("TEXT11"));
	
	CharacterOverlay = CreateWidget<UCharacterOverlay>(TargetController, CharacterOverlayClass);

	UE_LOG(LogTemp, Warning, TEXT("TEXT22"));
	
	if (!CharacterOverlay) return;
	CharacterOverlay->AddToViewport();

	TargetCharacter->Attach(CharacterOverlay);
	TargetCharacter->GetCombatComponent()->Attach(CharacterOverlay);
	TargetCharacter->GetStatusEffectComponent()->Attach(CharacterOverlay);

	UE_LOG(LogTemp, Warning, TEXT("TEXT33"));
}

void UCharacterControlMapper::Disconnect()
{
	if (!CharacterOverlay) return;
	CharacterOverlay->BeginDestroy();
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
