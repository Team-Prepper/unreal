
#include "InteractableActor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/Prepper.h"
#include "Prepper/Character/PlayerCharacter.h"


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

