#include "ItemBackpack.h"
#include "Prepper/Prepper.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Object/OpenedInventory.h"
#include "Sound/SoundCue.h"


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

void AItemBackpack::Attach(IObserver<UMapInventory>* Observer)
{
	Observers.insert(Observer);
	Observer->Update(*Inventory);
}

void AItemBackpack::Detach(IObserver<UMapInventory>* Observer)
{
	Observers.erase(Observer);
}

void AItemBackpack::Notify()
{
	std::ranges::for_each(Observers, [&](IObserver<UMapInventory>* Observer) {
		Observer->Update(*Inventory);
	});
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
	if(!Target) return;
	
	PlayerOwnerCharacter = Target;
	SetOwner(Target);
	
	Target->EquipBackpack(this);
	Target->AttachActorAtSocket(FName("BackpackSocket"), this);
	
	
	if(IsOpened)
		HideInventory();
	
	if (!EquipSound) return;
	
	UGameplayStatics::PlaySoundAtLocation(
		this,
		EquipSound,
		GetActorLocation()
	);
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
	case EBackpackState::EBS_OpenInventory:
		UE_LOG(LogTemp,Warning,TEXT("Backpack dropped"));
		BackpackPhysicsActive(false);

		EnableCustomDepth(true);
		SetActorEnableCollision(true);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		AreaSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		
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

	// 인벤토리 테이블 생성
	//AttachToActor(OpenedInventory, FAttachmentTransformRules::KeepWorldTransform);

	const UStaticMeshSocket* TargetSocket = OpenedInventory->GetMesh()->GetSocketByName(TEXT("BackpackSocket"));
	if(TargetSocket)
	{
		TargetSocket->AttachActor(this, OpenedInventory->GetMesh());
	}
	
	SetBackpackState(EBackpackState::EBS_OpenInventory);
}

void AItemBackpack::HideInventory()
{
	UE_LOG(LogTemp,Warning,TEXT("Hide Inventory"));
	if(!OpenedInventory) return;
	OpenedInventory->CloseInventory();
	OpenedInventory->Destroy();
	IsOpened = false;
}
