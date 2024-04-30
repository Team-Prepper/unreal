#include "ItemBackpack.h"
#include "Prepper/Prepper.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


AItemBackpack::AItemBackpack()
{
	PrimaryActorTick.bCanEverTick = false;

	BackpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackpackMesh"));
	SetRootComponent(BackpackMesh);

	BackpackMesh->SetCollisionResponseToAllChannels(ECR_Block);
	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	BackpackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);
}

void AItemBackpack::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AItemBackpack::BeginPlay()
{
	Super::BeginPlay();

	CustomDepthColor = CUSTOM_DEPTH_MINT;

	BackpackMesh->SetCustomDepthStencilValue(CustomDepthColor);
	BackpackMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);

	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBackpack::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItemBackpack::OnSphereEndOverlap);
	}
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
		Target->EquipBackpack(this);
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
