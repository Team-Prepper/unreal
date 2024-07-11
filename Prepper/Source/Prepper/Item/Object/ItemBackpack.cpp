#include "ItemBackpack.h"
#include "Prepper/Prepper.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Object/OpenedInventory.h"


AItemBackpack::AItemBackpack()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BackpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackpackMesh"));
	SetRootComponent(BackpackMesh);

	BackpackMesh->SetCollisionResponseToAllChannels(ECR_Block);
	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	BackpackMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BackpackMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	BackpackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackpackMesh->SetRenderCustomDepth(true);
	BackpackMesh->SetCustomDepthStencilValue(CustomDepthColor);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);

	Inventory = CreateDefaultSubobject<UMapInventory>(TEXT("Inventory"));
	Inventory->SetIsReplicated(true);
}

void AItemBackpack::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItemBackpack, BackpackState);
	DOREPLIFETIME(AItemBackpack, PlayerOwnerCharacter);
	DOREPLIFETIME(AItemBackpack, IsOpened);
}

void AItemBackpack::BeginPlay()
{
	Super::BeginPlay();

	CustomDepthColor = CUSTOM_DEPTH_MINT;

	BackpackMesh->SetCustomDepthStencilValue(CustomDepthColor);
	BackpackMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);
}

void AItemBackpack::EnableCustomDepth(bool bEnable)
{
	if(BackpackMesh)
	{
		BackpackMesh->SetRenderCustomDepth(bEnable);
	}
}

void AItemBackpack::Interaction(APlayerCharacter* Target)
{
	if(Target)
	{
		PlayerOwnerCharacter = Target;
		Target->EquipBackpack(this);
		SetOwner(Target);
	}
}

void AItemBackpack::Dropped()
{
	// TODO 
}


void AItemBackpack::OnRep_BackpackState()
{
	OnBackPackState();
}

void AItemBackpack::OnBackPackState()
{
	switch (BackpackState)
	{
	case EBackpackState::EBS_Equipped:
		BackpackMesh->SetSimulatePhysics(false);
		BackpackMesh->SetEnableGravity(false);
		EnableCustomDepth(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EBackpackState::EBS_Dropped:
		BackpackMesh->SetSimulatePhysics(true);
		BackpackMesh->SetEnableGravity(true);
		BackpackMesh->SetCollisionResponseToAllChannels(ECR_Block);
		BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		BackpackMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		BackpackMesh->SetCustomDepthStencilValue(CustomDepthColor);
		BackpackMesh->MarkRenderStateDirty();
		EnableCustomDepth(true);

		if (HasAuthority())
		{
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		break;

	default:
		break;
	}
}

void AItemBackpack::SetBackpackState(EBackpackState NewBackpackState)
{
	BackpackState = NewBackpackState;
	OnBackPackState();
}

void AItemBackpack::MulticastToggleInventory_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("Backpack : Toggle"));
	if(IsOpened)
	{
		HideInventory();
	}
	else
	{
		ShowInventory();
	}
	IsOpened = !IsOpened;
}

void AItemBackpack::ToggleInventory()
{
	MulticastToggleInventory();
}

void AItemBackpack::ShowInventory()
{
	UE_LOG(LogTemp,Warning,TEXT("Show Inventory"));
	UWorld* World = GetWorld();
	if(!World) return;
	if(!PlayerOwnerCharacter) return;
	if(!OpenedInventoryClass) return;

	FRotator SpawnRotation(0.0f, GetActorRotation().Yaw, 0.0f);
	OpenedInventory = World->SpawnActor<AOpenedInventory>(
		OpenedInventoryClass,
		GetActorLocation() + GetActorForwardVector() * 200.0f + GetActorLocation().DownVector * 200.0f,
		SpawnRotation
		);
	OpenedInventory->SetTargetInventory(Inventory);
}

void AItemBackpack::HideInventory()
{
	UE_LOG(LogTemp,Warning,TEXT("Hide Inventory"));
	if(!OpenedInventory) return;
	OpenedInventory->Destroy();
}
