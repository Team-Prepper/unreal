
#include "InteractableItem.h"
#include "Prepper/Prepper.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AInteractableItem::AInteractableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(ItemMesh);
	
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetRenderCustomDepth(true);
	ItemMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_PURPLE);
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);
}

void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ItemMesh)
	{
		ItemMesh->AddWorldRotation(FRotator(0.f, BaseTurnRate * DeltaTime, 0.f));
	}
}

void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(false);
	}
}


void AInteractableItem::Interaction(APlayerCharacter* Target)
{
	Target->AddItem(ItemCode);
	DestroyInteractionItem();
}

void AInteractableItem::DestroyInteractionItem()
{
	if(HasAuthority())
	{
		MulticastDestroyInteractionItem();
	}
	else
	{
		ServerDestroyInteractionItem();
	}
}

void AInteractableItem::ServerDestroyInteractionItem_Implementation()
{
	MulticastDestroyInteractionItem();
}

void AInteractableItem::MulticastDestroyInteractionItem_Implementation()
{
	Destroy();
}

void AInteractableItem::Destroyed()
{
	Super::Destroyed();

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}


