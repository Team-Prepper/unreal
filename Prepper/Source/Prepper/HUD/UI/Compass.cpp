// Fill out your copyright notice in the Description page of Project Settings.


#include "Compass.h"

#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Prepper/Character/PlayerCharacter.h"

void UCompass::NativeConstruct()
{
	Super::NativeConstruct();
	
	CompassOffset = GetImageWidth();
}

void UCompass::PossessPlayer()
{
	if(!GetOwningPlayerPawn()) return;
}

void UCompass::SetDirection()
{
	if(PlayerCam == nullptr)
	{
		PlayerCam = Cast<IControllable>(GetOwningPlayerPawn())->GetFollowCamera();
		UE_LOG(LogTemp,Warning,TEXT("Player Cam = nullptr"));
		return;
	}
	float YawValue = -10 * PlayerCam->GetComponentRotation().Yaw - (CompassOffset/2);

	if(!CompassPoint) return;
	
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CompassPoint->Slot);
	if(!CanvasSlot) return;
	
	CanvasSlot->SetPosition(FVector2D(YawValue,0));
}

float UCompass::GetImageWidth() const
{
	if(!CompassPoint) return 0.0f;
	
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CompassPoint->Slot);
	if(!CanvasSlot) return 0.0f;
	
	return CanvasSlot->GetSize().X;
	
}