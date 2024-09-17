
#include "InteractableActor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/Prepper.h"
#include "Prepper/Character/PlayerCharacter.h"



AInteractableActor::AInteractableActor()
{
	ColliderChannel.SetAllChannels(ECR_Block);
	ColliderChannel.SetResponse(ECC_Pawn, ECR_Ignore);
	ColliderChannel.SetResponse(ECC_Camera, ECR_Ignore);
	ColliderChannel.SetResponse(ECC_Vehicle, ECR_Ignore);
	
	TriggerChannel.SetAllChannels(ECR_Overlap);
	TriggerChannel.SetResponse(ECC_Pawn, ECR_Ignore);
	TriggerChannel.SetResponse(ECC_Camera, ECR_Ignore);

}

void AInteractableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetComponents<TObjectPtr<UMeshComponent>>(MeshComponents);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	if(AreaSphere)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionObjectType(ECC_InteractMesh);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(false);
	}
}

void AInteractableActor::ShowPickUpWidget(bool bShowWidget)
{
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(bShowWidget);
	}
}

void AInteractableActor::ToggleOutline(const bool bEnable)
{
	UE_LOG(LogTemp,Warning, TEXT("MeshCompCnt: %d"), MeshComponents.Num());
	
	for (const TObjectPtr<UMeshComponent> Mesh : MeshComponents)
	{
		Mesh->SetRenderCustomDepth(bEnable);
	}
}

void AInteractableActor::ToggleTrigger(const bool bEnable)
{
	if (!bEnable)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
	}
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToChannels(TriggerChannel);
}

