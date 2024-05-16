// Fill out your copyright notice in the Description page of Project Settings.


#include "Compass.h"

#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"

void UCompass::NativeConstruct()
{
	Super::NativeConstruct();
	
	CompassOffset = GetImageWidth();
}

void UCompass::PossessPlayer()
{
	if(GetOwningPlayerPawn())
	{
		PlayerCam = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetFollowCamera();
	}
}

void UCompass::SetDirection()
{
	if(PlayerCam == nullptr)
	{
		PlayerCam = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetFollowCamera();
		UE_LOG(LogTemp,Warning,TEXT("Player Cam = nullptr"));
		return;
	}
	float YawValue = PlayerCam->GetComponentRotation().Yaw;
	YawValue *= -10;
	YawValue -= (CompassOffset/2);

	if(CompassPoint)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CompassPoint->Slot);
		if(CanvasSlot)
		{
			CanvasSlot->SetPosition(FVector2D(YawValue,0));
		}
	}
}

float UCompass::GetImageWidth() const
{
	if(CompassPoint)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CompassPoint->Slot);
		if(CanvasSlot)
		{
			return CanvasSlot->GetSize().X;
		}
	}
	return 0.0f;
}
