// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Prepper.h"
#include "Prepper/Interfaces/Interactable.h"
#include "Prepper/Item/AInteractableActor.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInteractionComponent, CurInteractableItem);
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FHitResult HitResult;
	TraceInteractionItem(HitResult);
}

void UInteractionComponent::TraceInteractionItem(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TraceRange;

		// USphereComponent 충돌 채널 설정
		ECollisionChannel CollisionChannel = ECC_InteractMesh; 

		bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			CollisionChannel
		);

		if (bHitSomething)
		{
			SetItemInteractable(TraceHitResult.GetActor());
		}
		else
		{
			SetItemInteractable(nullptr);
		}
	}
}

void UInteractionComponent::SetItemInteractable(AActor* InteractableItem)
{
	TScriptInterface<IInteractable> LastInteractableItem = CurInteractableItem;
	
	
	if(LastInteractableItem && InteractableItem == nullptr)
	{
		LastInteractableItem->ShowPickUpWidget(false);
		LastInteractableItem = nullptr;
		if(!Character->HasAuthority())
		{
			ServerSetItemInteractable(nullptr);
		}
		return;
	}
	
	CurInteractableItem = TScriptInterface<IInteractable>(InteractableItem);
	if(LastInteractableItem != CurInteractableItem)
	{
		if(!Character->HasAuthority())
		{
			ServerSetItemInteractable(InteractableItem);
		}
		if(Character->IsLocallyControlled())
		{
			if(LastInteractableItem)
			{
				LastInteractableItem->ShowPickUpWidget(false);
			}
			if(CurInteractableItem)
			{
				CurInteractableItem->ShowPickUpWidget(true);
			}
		}
	}
	
}

void UInteractionComponent::ServerSetItemInteractable_Implementation(AActor* InteractableItem)
{
	if(InteractableItem == nullptr)
	{
		CurInteractableItem = nullptr;
		return;
	}
	CurInteractableItem = TScriptInterface<IInteractable>(InteractableItem);
}
