#include "WeaponActor.h"

#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
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
	EnableCustomDepth(false);
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);;
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	AreaSphere->SetCollisionObjectType(ECC_InteractMesh);
	
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);

	// 노이즈 생성 컴포넌트 추가
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));
}

void AWeaponActor::PlayEquipWeaponSound()
{
	if (!EquipSound) return;
	if (!PlayerOwnerCharacter) return;
	
	UGameplayStatics::PlaySoundAtLocation(
		PlayerOwnerCharacter, EquipSound,
		PlayerOwnerCharacter->GetActorLocation()
	);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	CustomDepthColor = CUSTOM_DEPTH_MINT;

	WeaponMesh->SetCustomDepthStencilValue(CustomDepthColor);
	StaticWeaponMesh->SetCustomDepthStencilValue(CustomDepthColor);
	WeaponMesh->MarkRenderStateDirty();
	StaticWeaponMesh->MarkRenderStateDirty();
	
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		SetWeaponState(EWeaponState::EWS_Dropped);
	}
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponActor, WeaponState);
	DOREPLIFETIME_CONDITION(AWeaponActor, bUseServerSideRewind, COND_OwnerOnly);
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
		WeaponHandler = nullptr;
		return;
	}
}

void AWeaponActor::SetWeaponHandler(IWeaponHandler* NewOwner)
{
	WeaponHandler = NewOwner;
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
			CurWeaponState = EWeaponState::EWS_Equipped;
			break;
		case EWeaponState::EWS_Holstered:
			OnEquippedSecondary();
			CurWeaponState = EWeaponState::EWS_Holstered;
			break;
		case EWeaponState::EWS_Dropped:
			OnDropped();
			CurWeaponState = EWeaponState::EWS_Dropped;
			break;
		default:
			break;
	}
}

void AWeaponActor::OnEquipped()
{
	ShowPickUpWidget(false);
	WeaponPhysicsActive(false);

	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ?
		Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;

	if (!PlayerOwnerCharacter) return;

	PlayerOwnerCharacter->AttachActorAtSocket(AttachSocketName(), this);
	PlayEquipWeaponSound();
	UE_LOG(LogTemp, Warning , TEXT("WEAPON : WEAPON EQUIPPED"));
		
	if (!bUseServerSideRewind) return;
	
	PlayerOwnerController = PlayerOwnerController == nullptr ?
		Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
	if (PlayerOwnerController && HasAuthority() && !PlayerOwnerController->HighPingDelegate.IsBound())
	{
		PlayerOwnerController->HighPingDelegate.AddDynamic(this, &AWeaponActor::OnPingTooHigh);
	}
}

void AWeaponActor::OnDropped()
{
	UE_LOG(LogTemp, Warning , TEXT("WEAPON : WEAPON DROPPED"));
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);

	WeaponPhysicsActive(true);
	WeaponMesh->DetachFromComponent(DetachRules);

	if(OnDroppedWeapon.IsBound())
		OnDroppedWeapon.Broadcast();
	OnDroppedWeapon.Clear();

	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ?
		Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;
	if (!PlayerOwnerCharacter) return;
	
	PlayerOwnerController = PlayerOwnerController == nullptr ?
		Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
	if (PlayerOwnerController && HasAuthority() && PlayerOwnerController->HighPingDelegate.IsBound())
	{
		PlayerOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeaponActor::OnPingTooHigh);
	}
	
	SetOwner(nullptr);
	PlayerOwnerCharacter = nullptr;
	PlayerOwnerController = nullptr;
}

void AWeaponActor::OnEquippedSecondary()
{
	ShowPickUpWidget(false);
	WeaponPhysicsActive(false);
	
	PlayerOwnerCharacter = PlayerOwnerCharacter == nullptr ?
		Cast<APlayerCharacter>(GetOwner()) : PlayerOwnerCharacter;

	if (!PlayerOwnerCharacter) return;
	
	UE_LOG(LogTemp, Warning , TEXT("WEAPON : WEAPON SECONDARY"));
	PlayerOwnerCharacter->AttachActorAtSocket(HolsteredWeaponSocketName, this);
	PlayEquipWeaponSound();
	
	PlayerOwnerController = PlayerOwnerController == nullptr ?
		Cast<APrepperPlayerController>(PlayerOwnerCharacter->Controller) : PlayerOwnerController;
	if (PlayerOwnerController && HasAuthority() && PlayerOwnerController->HighPingDelegate.IsBound())
	{
		PlayerOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeaponActor::OnPingTooHigh);
	}
}

IWeaponHandler* AWeaponActor::GetWeaponHandler()
{
	if (WeaponHandler != nullptr) return WeaponHandler;
	
	return nullptr;
}

void AWeaponActor::OnPingTooHigh(bool bPingTooHigh)
{
	bUseServerSideRewind = !bPingTooHigh;
}


void AWeaponActor::GetCrosshair(float DeltaTime, bool bIsAiming, UTexture2D* &Center, UTexture2D* &Left,
	UTexture2D* &Right, UTexture2D* &Top, UTexture2D* &Bottom, float &Spread)
{
	Center = nullptr;
	Left = nullptr;
	Right = nullptr;
	Top = nullptr;
	Bottom = nullptr;
	Spread = 0.5f;
}

TArray<FVector_NetQuantize> AWeaponActor::GetTarget(FVector& HitTarget)
{
	TArray<FVector_NetQuantize> HitTargets;
	HitTargets.Add(HitTarget);
	return HitTargets;
}

void AWeaponActor::WeaponPhysicsActive(bool active)
{
	SetActorEnableCollision(active);
	WeaponMesh->SetSimulatePhysics(active);
	WeaponMesh->SetEnableGravity(active);
	StaticWeaponMesh->SetSimulatePhysics(false);
	StaticWeaponMesh->SetEnableGravity(false);
	EnableCustomDepth(active);

	if (active)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		AreaSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
		StaticWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		StaticWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		StaticWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

		return;
	}
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}