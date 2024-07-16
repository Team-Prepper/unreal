#include "CraftingTable.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"

ACraftingTable::ACraftingTable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	SetRootComponent(TableMesh);
	TableMesh->SetCollisionResponseToAllChannels(ECR_Block);
	TableMesh->SetRenderCustomDepth(true);
	TableMesh->SetCustomDepthStencilValue(CustomDepthColor);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);
}

void ACraftingTable::Interaction(APlayerCharacter* Target)
{
	MulticastInteraction(Target);
}

void ACraftingTable::MulticastInteraction_Implementation(APlayerCharacter* Target)
{
	if(!Target->IsLocallyControlled()) return;
	// DO SOMETHING
}

