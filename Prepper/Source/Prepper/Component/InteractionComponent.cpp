// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Prepper.h"
#include "Prepper/Interfaces/Interactable.h"
#include "Prepper/Item/AInteractableActor.h"
#include "DrawDebugHelpers.h"

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

		//---
		FColor TraceColor = bHitSomething ? FColor::Red : FColor::Green; // 충돌 시 빨강, 아니면 초록

		// 디버그 라인 그리기
		DrawDebugLine(
			GetWorld(),
			Start, // 시작 지점
			End, // 끝 지점
			TraceColor, // 라인의 색상
			false, // 지속적으로 그릴 것인지
			1.0f, // 라인이 화면에 유지되는 시간 (초 단위)
			0, // 디버그 라인을 그릴 두께
			.1f // 디버그 라인의 두께
		);
		//---

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
	AInteractableActor* LastInteractableItem = CurInteractableItem;
	if(LastInteractableItem)
	{
		LastInteractableItem->ShowPickUpWidget(false);
	}
	
	IInteractable* TheInterface = Cast<IInteractable>(InteractableItem);
	if (TheInterface == nullptr && LastInteractableItem != nullptr)
	{
		LastInteractableItem = nullptr;
		if(!Character->HasAuthority())
		{
			ServerSetItemInteractable(nullptr);
		}
		return;
	}
	
	CurInteractableItem = Cast<AInteractableActor>(InteractableItem);
	if(LastInteractableItem != CurInteractableItem)
	{
		ServerSetItemInteractable(InteractableItem);
		if(Character->IsLocallyControlled())
		{
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
	CurInteractableItem = Cast<AInteractableActor>(InteractableItem);
}
