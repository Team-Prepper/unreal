#include "WeaponActor.h"

#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Prepper.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeWeaponMesh"));
	StaticWeaponMesh->SetupAttachment(RootComponent);
	StaticWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponMesh->SetCustomDepthStencilValue(CustomDepthColor);
	StaticWeaponMesh->SetCustomDepthStencilValue(CustomDepthColor);
	WeaponMesh->MarkRenderStateDirty();
	StaticWeaponMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	AreaSphere->SetCollisionObjectType(ECC_InteractMesh);
	
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);

	// 노이즈 생성 컴포넌트 추가
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	CustomDepthColor = CUSTOM_DEPTH_MINT;

	WeaponMesh->SetCustomDepthStencilValue(CustomDepthColor);
	StaticWeaponMesh->SetCustomDepthStencilValue(CustomDepthColor);
	WeaponMesh->MarkRenderStateDirty();
	StaticWeaponMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);
	
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponActor, WeaponState);
}

void AWeaponActor::EnableCustomDepth(bool bEnable)
{
	if(WeaponMesh)
	{
		WeaponMesh->SetRenderCustomDepth(bEnable);
	}
	if(StaticWeaponMesh)
	{
		StaticWeaponMesh->SetRenderCustomDepth(bEnable);
	}
}

void AWeaponActor::OnRep_Owner()
{
	Super::OnRep_Owner();
	if(Owner == nullptr)
	{
		PlayerOwnerCharacter = nullptr;
		PlayerOwnerController = nullptr;
	}
}

void AWeaponActor::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	SetActorEnableCollision(true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	PlayerOwnerCharacter = nullptr;
	PlayerOwnerController = nullptr;
}

void AWeaponActor::Interaction(APlayerCharacter* Target)
{
	Target->EquipWeapon(this);
}

void AWeaponActor::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateSet();
}

// client
void AWeaponActor::OnRep_WeaponState()
{
	OnWeaponStateSet();
}

void AWeaponActor::OnWeaponStateSet()
{
	switch (WeaponState)
	{
		case EWeaponState::EWS_Equipped:
			OnEquipped();
			break;
		case EWeaponState::EWS_Holstered:
			OnEquippedSecondary();
			break;
		case EWeaponState::EWS_Dropped:
			OnDropped();
			break;
		default:
			break;
	}
}

void AWeaponActor::OnEquipped()
{
	UE_LOG(LogTemp, Warning , TEXT("WEAPON : WEAPON EQUIPPED"));
	ShowPickUpWidget(false);
	SetActorEnableCollision(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticWeaponMesh->SetSimulatePhysics(false);
	StaticWeaponMesh->SetEnableGravity(false);
	StaticWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EnableCustomDepth(false);

	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;
	if (PlayerOwnerCharacter && bUseServerSideRewind)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController && HasAuthority() && !PlayerOwnerController->HighPingDelegate.IsBound())
		{
			PlayerOwnerController->HighPingDelegate.AddDynamic(this, &AWeaponActor::OnPingTooHigh);
		}
	}
}

void AWeaponActor::OnDropped()
{
	SetActorEnableCollision(true);
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	StaticWeaponMesh->SetSimulatePhysics(false);
	StaticWeaponMesh->SetEnableGravity(false);
	StaticWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	EnableCustomDepth(true);

	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;
	if (PlayerOwnerCharacter)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController && HasAuthority() && PlayerOwnerController->HighPingDelegate.IsBound())
		{
			PlayerOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeaponActor::OnPingTooHigh);
		}
	}
}

void AWeaponActor::OnEquippedSecondary()
{
	ShowPickUpWidget(false);
	EnableCustomDepth(false);
	SetActorEnableCollision(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticWeaponMesh->SetSimulatePhysics(false);
	StaticWeaponMesh->SetEnableGravity(false);
	StaticWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;
	if (PlayerOwnerCharacter)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController && HasAuthority() && PlayerOwnerController->HighPingDelegate.IsBound())
		{
			PlayerOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeaponActor::OnPingTooHigh);
		}
	}
}

void AWeaponActor::OnPingTooHigh(bool bPingTooHigh)
{
	bUseServerSideRewind = !bPingTooHigh;
}


void AWeaponActor::GetCrosshair(UTexture2D* &Center, UTexture2D* &Left, UTexture2D* &Right, UTexture2D* &Top, UTexture2D* &Bottom)
{
	Center = nullptr;
	Left = nullptr;
	Right = nullptr;
	Top = nullptr;
	Bottom = nullptr;
}

FName AWeaponActor::AttachSocketName()
{
	return FName("ddd");
}

TArray<FVector_NetQuantize> AWeaponActor::GetTarget(FVector& HitTarget)
{
	TArray<FVector_NetQuantize> HitTargets;
	HitTargets.Add(HitTarget);
	return HitTargets;
}

bool AWeaponActor::IsMeleeWeapon()
{
	return GetWeaponType() == EWeaponType::EWT_MeleeWeaponBlunt || GetWeaponType() == EWeaponType::EWT_MeleeWeaponSword;
}