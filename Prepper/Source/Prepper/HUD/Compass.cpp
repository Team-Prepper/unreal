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
	PossessPlayer();
}

void UCompass::SetDirection()
{
	if(!Cast<APrepperPlayerController>(GetOwningPlayer())) {
		GetWorld()->GetTimerManager().ClearTimer(SetDirectionTimerHandle);
		PossessPlayer();
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

void UCompass::PossessPlayer()
{
	if(GetOwningPlayerPawn())
	{
		PlayerCam = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetFollowCamera();
		if(PlayerCam)
		{
			GetWorld()->GetTimerManager().SetTimer(
				SetDirectionTimerHandle,
				this,
				&UCompass::SetDirection,
				0.01,
				true
			);
			return;
		}
	}
	GetWorld()->GetTimerManager().SetTimer(
	PlayerPossessTimerHandle,
	this,
	&UCompass::PossessPlayer,
	1,
	false
	);
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
