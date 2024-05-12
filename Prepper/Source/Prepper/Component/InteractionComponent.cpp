// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Prepper/Prepper.h"
#include "Prepper/Interfaces/Interactable.h"
#include "Prepper/Item/AInteractableActor.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Character);
		
		bool bHitSomething = GetWorld()->LineTraceSingleByObjectType(
			TraceHitResult,
			Start,
			End,
			FCollisionObjectQueryParams(ECC_InteractMesh), // 동적 물체와 충돌 검사
			QueryParams
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
	if(CurInteractableItem != nullptr && InteractableItem == nullptr)
	{
		CurInteractableItem->ShowPickUpWidget(false);
		if(Character->IsLocallyControlled())
		{
			CurInteractableItem = nullptr;
		}
		if(!Character->HasAuthority() && Character->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Null Interaction"));
			ServerSetItemInteractable(nullptr);
		}
		return;
	}
	
	TScriptInterface<IInteractable> NewInteractableItem = TScriptInterface<IInteractable>(InteractableItem);
	if(NewInteractableItem != CurInteractableItem)
	{
		if(!Character->HasAuthority() && Character->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Interaction Item"));
			ServerSetItemInteractable(InteractableItem);
		}
		if(Character->IsLocallyControlled())
		{
			if(CurInteractableItem)
			{
				CurInteractableItem->ShowPickUpWidget(false);
			}
			CurInteractableItem = TScriptInterface<IInteractable>(InteractableItem);
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
