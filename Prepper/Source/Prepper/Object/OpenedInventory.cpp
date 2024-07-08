#include "OpenedInventory.h"

#include "GameFramework/SpringArmComponent.h"
#include "Prepper/Component/CustomCameraComponent.h"

AOpenedInventory::AOpenedInventory()
{
	PrimaryActorTick.bCanEverTick = false;

	InventoryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InventoryMesh"));
	SetRootComponent(InventoryMesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultCamArmLength;

	InventoryCam = CreateDefaultSubobject<UCustomCameraComponent>(TEXT("InvenCam"));
	InventoryCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	InventoryCam->bUsePawnControlRotation = false;
}

void AOpenedInventory::BeginPlay()
{
	Super::BeginPlay();

	/* TODO
	 * 인벤토리 추적 : Backpack에 Component로 존재  
	 * 인벤토리에 있는 아이템 Spawn
	 * 해당 아이템은 상호작용시 상호작용 HUD를 생성 
	 */
}

