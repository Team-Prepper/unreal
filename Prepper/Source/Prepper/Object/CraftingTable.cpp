#include "CraftingTable.h"

#include "Prepper/Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

ACraftingTable::ACraftingTable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	SetRootComponent(TableMesh);
	TableMesh->SetCollisionResponseToAllChannels(ECR_Block);
	TableMesh->SetRenderCustomDepth(true);
	TableMesh->SetCustomDepthStencilValue(CustomDepthColor);

	AreaBox = CreateDefaultSubobject<UBoxComponent>("AreaBox");
	AreaBox->SetupAttachment(RootComponent);
	AreaBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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

