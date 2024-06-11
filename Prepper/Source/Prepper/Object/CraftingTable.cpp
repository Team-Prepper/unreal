#include "CraftingTable.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/HUD/Item/CraftUI.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/_Base/UISystem/UIManager.h"

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
	// On Server
	MulticastInteraction(Target);
}

void ACraftingTable::MulticastInteraction_Implementation(APlayerCharacter* Target)
{
	if(!Target->IsLocallyControlled()) return;
	ShowCraftingWidget(Target);
	//Target->OpenCraftingTable();
}

void ACraftingTable::ShowCraftingWidget(APlayerCharacter* Target)
{
	UCraftUI* CraftUI = UIManager::GetInstance()->OpenGUI<UCraftUI>(Cast<APlayerController>(Target->GetController()), TEXT("CraftUI"));
	CraftUI->SetTargetInventory(Target->Inven);
}

