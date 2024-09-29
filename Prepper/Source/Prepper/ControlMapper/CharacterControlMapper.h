#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "UObject/Interface.h"
#include "ControlMapper.h"
#include "CharacterControlMapper.generated.h"

class UCharacterOverlay;
class APlayerCharacter;
class UUserWidget;

UCLASS(Blueprintable, BlueprintType)
class PREPPER_API UCharacterControlMapper: public UObject, public IControlMapper
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UUserWidget> CharacterOverlayClass;
	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	
	void ToggleControlWidget(bool Toggle, APlayerController* TargetController);
	
	UCharacterControlMapper():
		CharacterOverlay(nullptr), TargetCharacter(nullptr) { } ;
	UPROPERTY()
	APlayerCharacter* TargetCharacter;
	
	virtual void Connect(APlayerController* TargetController) override;
	virtual void Disconnect() override;
	
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;

	virtual void ShiftPressed() override;
	virtual void ShiftReleased() override;

	virtual void SpacePressed() override;
	virtual void SpaceReleased() override;
	virtual void EPressed() override;
	virtual void RPressed() override;

	virtual void ControlPressed() override;

	virtual void MouseLeftPressed() override;
	virtual void MouseLeftReleased() override;
	virtual void MouseRightPressed() override;
	virtual void MouseRightReleased() override;

	virtual UCameraComponent* GetFollowCamera() override;
};