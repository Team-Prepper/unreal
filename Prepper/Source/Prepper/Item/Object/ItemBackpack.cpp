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

		if(IsOpened)
			HideInventory();
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
		UE_LOG(LogTemp,Warning,TEXT("Backpack equipped"));
		BackpackPhysicsActive(false);
		break;
	case EBackpackState::EBS_Dropped:
		UE_LOG(LogTemp,Warning,TEXT("Backpack dropped"));
		BackpackPhysicsActive(true);
		SetOwner(nullptr);
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

void AItemBackpack::BackpackPhysicsActive(bool active)
{
	SetActorEnableCollision(active);
	BackpackMesh->SetSimulatePhysics(active);
	BackpackMesh->SetEnableGravity(active);
	EnableCustomDepth(active);

	if (active)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		AreaSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

		BackpackMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BackpackMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		BackpackMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		BackpackMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		BackpackMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

		return;
	}
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackpackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBackpack::OpenInventory()
{
	ShowInventory();
	IsOpened = true;
}

void AItemBackpack::ToggleInventory()
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
	
	AttachToActor(OpenedInventory, FAttachmentTransformRules::KeepWorldTransform /* , socketName*/ );
	
	SetBackpackState(EBackpackState::EBS_Dropped);
	
		
}

void AItemBackpack::HideInventory()
{
	UE_LOG(LogTemp,Warning,TEXT("Hide Inventory"));
	if(!OpenedInventory) return;
	OpenedInventory->CloseInventory();
	OpenedInventory->Destroy();
	IsOpened = false;
}
