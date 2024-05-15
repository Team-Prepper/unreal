#include "PlayerCharacter.h"	
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Prepper.h"
#include "Prepper/Component/CombatComponent.h"
#include "Prepper/Component/StatusEffectComponent.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/PlayerState/DeathMatchPlayerState.h"
#include "Prepper/Weapon/WeaponActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Prepper/Component/InteractionComponent.h"
#include "Prepper/Item/Object/ItemBackpack.h"
#include "Sound/SoundCue.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Prepper/Item/MapInventory.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	CrouchCamOffset = 30.f;
	DefaultCamOffset = 100.f;
	CrouchCamArmLength = 200.f;
	DefaultCamArmLength = 350.f;
	InterpSpeed = 5.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = DefaultCamArmLength;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	Hair->SetRelativeRotation(FRotator(-90.f, 0, 90.f));
	Hair->SetupAttachment(GetMesh(),FName("Head"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractComponent"));
	InteractionComponent->SetIsReplicated(true);

	StatusEffect = CreateDefaultSubobject<UStatusEffectComponent>(TEXT("StatusEffectComponet"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	bBeforeSeat = false;

	// 노이즈 생성 컴포넌트 추가
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayerCharacter, bDisableGamePlay);
	DOREPLIFETIME(APlayerCharacter, PlayerMovementState);
	DOREPLIFETIME(APlayerCharacter, EquippedBackpack);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->Character = this;
	}
	if(StatusEffect)
	{
		StatusEffect->Character = this;
	}
	if(InteractionComponent)
	{
		InteractionComponent->Character = this;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PrepperPlayerController = Cast<APrepperPlayerController>(Controller);
	
	if(PrepperPlayerController)
	{
		//Inven = NewObject<UMapInventory>(GetWorld(), UMapInventory::StaticClass());
		Inven.TryAddItem("Milk");
		Inven.TryAddItem("Milgaru");
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);

	RotateInPlace(DeltaTime);
	HideCamIfCharacterClose();

	FVector CurrentLocation = CameraBoom->GetRelativeLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetSpringArmLocation, DeltaTime, InterpSpeed);
	CameraBoom->SetRelativeLocation(NewLocation);
	
	float CurrentArmLength = CameraBoom->TargetArmLength;
	float NewArmLength = FMath::FInterpTo(CurrentArmLength, TargetArmLength, DeltaTime, InterpSpeed);
	CameraBoom->TargetArmLength = NewArmLength;
	
	PollInit();
	if (PlayerMovementState == EPlayerMovementState::EPMS_Sprint)
	{
		MakeNoise(1, nullptr, FVector::ZeroVector);
	}
}

void APlayerCharacter::RotateInPlace(float DeltaTime)
{
	if(bDisableGamePlay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	if(GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
}


void APlayerCharacter::ShiftPressed()
{
	if(bDisableGamePlay) return;
	SetPlayerMovementState(EPlayerMovementState::EPMS_Sprint);
}

void APlayerCharacter::ShiftReleased()
{
	if(bDisableGamePlay) return;
	SetPlayerMovementState(EPlayerMovementState::EPMS_Idle);
}

void APlayerCharacter::PlayFireMontage(bool bAiming)
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FName SectionName;
	if(Combat->EquippedRangeWeapon)
	{
		if(AnimInstance && FireWeaponMontage)
		{
			AnimInstance->Montage_Play(FireWeaponMontage);
			SectionName = bAiming ? FName("FireAim") : FName("FireHip");
			AnimInstance->Montage_JumpToSection(SectionName);
		}
	}
	else
	{
		if(AnimInstance && MeleeWeaponMontage)
		{
			AnimInstance->Montage_Play(MeleeWeaponMontage);
			SectionName = bAiming ? FName("Attack1") : FName("Attack2");
			AnimInstance->Montage_JumpToSection(SectionName);
		}
	}
	
}

void APlayerCharacter::PlayReloadMontage(const FName& SectionName)
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if(AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void APlayerCharacter::PlaySwapMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SwapMontage)
	{
		AnimInstance->Montage_Play(SwapMontage);
	}
}

void APlayerCharacter::PlayHitReactMontage()
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("HitFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}


void APlayerCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);
}

void APlayerCharacter::UpdateHUDHealth()
{
	// 본인의 플레이어가 다른이로부터 피해를 받았을 때 내 화면의 GUI를 갱신하기 위한 코드
	PrepperPlayerController = PrepperPlayerController == nullptr ?  Cast<APrepperPlayerController>(Controller) : PrepperPlayerController;
	if(PrepperPlayerController)
	{
		PrepperPlayerController->SetHUDHealth(CurrentHealth, MaxHealth);
	}
}

void APlayerCharacter::PollInit()
{
	if(DeathMatchPlayerState == nullptr)
	{
		DeathMatchPlayerState = GetPlayerState<ADeathMatchPlayerState>();
		if(DeathMatchPlayerState)
		{
			DeathMatchPlayerState->AddToScore(0.f);
			DeathMatchPlayerState->AddToDefeats(0);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if(bDisableGamePlay) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Elim()
{
	if (Combat && Combat->EquippedWeapon)
	{
		if(Combat->EquippedWeapon)
		{
			Combat->EquippedWeapon->Dropped();
			Combat->EquippedWeapon = nullptr;
		}
		if(Combat->SecondaryWeapon)
		{
			Combat->SecondaryWeapon->Dropped();
			Combat->SecondaryWeapon = nullptr;
		}
		
	}
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&APlayerCharacter::ElimTimerFinished,
		ElimDelay
	);
}

void APlayerCharacter::MulticastElim()
{
	if(PrepperPlayerController)
	{
		PrepperPlayerController->SetHUDWeaponAmmo(0);
	}
	Super::MulticastElim();
	bDisableGamePlay = true;
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}
	bool bHideSniperScope = IsLocallyControlled() && 
		Combat && 
		Combat->bAiming && 
		Combat->EquippedWeapon && 
		Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;
	if (bHideSniperScope)
	{
		ShowSniperScopeWidget(false);
	}
}

void APlayerCharacter::ElimTimerFinished()
{
	ADeathMatchGameMode* DeathMatchGameMode = GetWorld()->GetAuthGameMode<ADeathMatchGameMode>();
	if (DeathMatchGameMode)
	{
		DeathMatchGameMode->RequestRespawn(this, Controller);
	}
	if(PrepperPlayerController)
	{
		PrepperPlayerController->TargetControllerable = nullptr;
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::SpacePressed()
{
	if(bDisableGamePlay) return;
	Jump();
}

void APlayerCharacter::SpaceReleased()
{
	if(bDisableGamePlay) return;
	StopJumping();
}

void APlayerCharacter::EPressed()
{
	if(bDisableGamePlay) return;
	TScriptInterface<IInteractable> InteractableItem;
	if(InteractionComponent)
	{
		InteractableItem = InteractionComponent->CurInteractableItem;
	}

	/* Interaction */
	if(InteractableItem)
	{
		UE_LOG(LogTemp, Warning , TEXT("INTERACTABLE ITEM"));
		InteractionComponent->CurInteractableItem->Interaction(this);
		return;
	}

	/* Swap */
	if (Combat && Combat->ShouldSwapWeapons())
	{
		UE_LOG(LogTemp, Warning , TEXT("COMBAT COMP : SWAP"));
		Combat->SwapWeapons();
	}
	bool bSwap = Combat->ShouldSwapWeapons() &&
			Combat->CombatState == ECombatState::ECS_Unoccupied &&
			InteractableItem == nullptr;

	if (bSwap)
	{
		PlaySwapMontage();
		bFinishedSwapping = false;
	}
}

void APlayerCharacter::EquipWeapon(AWeaponActor* Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Equip Weapon"));
	if(bDisableGamePlay) return;
	if(Combat)
	{
		Combat->EquipWeapon(Weapon);
	}
}

void APlayerCharacter::ControlPressed()
{
	if(bDisableGamePlay) return;
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void APlayerCharacter::RPressed()
{
	if(bDisableGamePlay) return;
	if(Combat)
	{
		Combat->Reload();
	}
}

void APlayerCharacter::MouseRightPressed()
{
	if(bDisableGamePlay) return;
	if(Combat)
	{
		Combat->SetAiming(true);
	}
}

void APlayerCharacter::MouseRightReleased()
{
	if(bDisableGamePlay) return;
	if(Combat)
	{
		Combat->SetAiming(false);
	}
}

void APlayerCharacter::MouseLeftPressed()
{
	if(bDisableGamePlay) return;
	if(Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void APlayerCharacter::MouseLeftReleased()
{
	if(bDisableGamePlay) return;
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void APlayerCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if(AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [ 270, 360) -> [-90,0)
		const FVector2d InRange(270.f, 360.f);
		const FVector2d OutRange(-90.f,0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
}

float APlayerCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void APlayerCharacter::EquipBackpack(AItemBackpack* BackpackToEquip)
{
	if(BackpackToEquip == nullptr) return;

	EquippedBackpack = BackpackToEquip;
	EquippedBackpack->SetBackpackState(EBackpackState::EBS_Equipped);

	AttachActorAtSocket(FName("BackpackSocket"), EquippedBackpack);

	if (EquippedBackpack->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquippedBackpack->EquipSound,
			GetActorLocation()
		);
	}
	
}

void APlayerCharacter::AttachActorAtSocket(const FName& SocketName, AActor* TargetActor)
{
	const USkeletalMeshSocket* TargetSocket = GetMesh()->GetSocketByName(SocketName);
	if(TargetSocket)
	{
		TargetSocket->AttachActor(TargetActor, GetMesh());
	}
}

void APlayerCharacter::OnRep_EquippedBackpack()
{
	if(EquippedBackpack)
	{
		EquipBackpack(EquippedBackpack);
	}
}

void APlayerCharacter::AimOffset(float DeltaTime)
{
	if(Combat && Combat->EquippedWeapon == nullptr) return;

	float Speed = CalculateSpeed();

	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if(Speed == 0.f && !bIsInAir) // Standing, not Jumping
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if(Speed > 0.f || bIsInAir) // Running or Jumping
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	CalculateAO_Pitch();
}

void APlayerCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}


void APlayerCharacter::SimProxiesTurn()
{
	if(Combat == nullptr || Combat -> EquippedWeapon == nullptr) return;

	bRotateRootBone = false;
	float Speed = CalculateSpeed();

	if(Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	
	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw =  UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if(FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if(ProxyYaw  > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < - TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void APlayerCharacter::Jump()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}else
	{
		Super::Jump();
	}
}

void APlayerCharacter::TurnInPlace(float DeltaTime)
{
	if(AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if(TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 10.f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void APlayerCharacter::ServerEquipButtonPressed_Implementation(AWeaponActor* Weapon)
{
	if (Combat)
	{
		if (Weapon)
		{
			Combat->EquipWeapon(Weapon);
		}
		else if (Combat->ShouldSwapWeapons())
		{
			Combat->SwapWeapons();
		}
	}
}

void APlayerCharacter::ServerConvertPlayerMovementState_Implementation(const EPlayerMovementState State)
{
	PlayerMovementState = State;
	MulticastConvertPlayerMovementState(State);
}

void APlayerCharacter::MulticastConvertPlayerMovementState_Implementation(const EPlayerMovementState State)
{
	PlayerMovementState = State;
	ConvertPlayerMovementState();
}


void APlayerCharacter::SetPlayerMovementState(const EPlayerMovementState State)
{
	if(IsLocallyControlled())
	{
		PlayerMovementState = State;
		ConvertPlayerMovementState();
		ServerConvertPlayerMovementState(State);
	}
}

void APlayerCharacter::ConvertPlayerMovementState()
{
	if(bBeforeSeat)
	{
		bBeforeSeat = false;
		SetActorEnableCollision(true);
		SetActorHiddenInGame(false);
		HidePlayerEquipment(false);
	}
	
	switch (PlayerMovementState)
	{
	case EPlayerMovementState::EPMS_Seat:
		bBeforeSeat = true;
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		HidePlayerEquipment(true);
		break;
	case EPlayerMovementState::EPMS_Aim:
		GetCharacterMovement()->MaxWalkSpeed = AimMovementSpeed;
		break;
	case EPlayerMovementState::EPMS_Sprint:
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;
	case EPlayerMovementState::EPMS_Idle:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	default:
		break;
	}
}


void APlayerCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
	TargetSpringArmLocation = (FVector(0.0f, 0.0f, CrouchCamOffset));
	TargetArmLength = CrouchCamArmLength;
}

void APlayerCharacter::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);

	TargetSpringArmLocation = (FVector(0.0f, 0.0f, DefaultCamOffset));
	TargetArmLength = DefaultCamArmLength;
}

void APlayerCharacter::HideCamIfCharacterClose()
{
	if (!IsLocallyControlled()) return;
	if((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CamThreshold)
	{
		HidePlayerMesh(true);
		
	}
	else
	{
		HidePlayerMesh(false);
		/*
		if(Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
		*/
	}
}


void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	ADeathMatchGameMode* DeathMatchGameMode = Cast<ADeathMatchGameMode>(UGameplayStatics::GetGameMode(this));
	bool bMatchNotInProgress = DeathMatchGameMode && DeathMatchGameMode->GetMatchState() != MatchState::InProgress;

	if(Combat && Combat->EquippedWeapon && bMatchNotInProgress)
	{
		Combat->EquippedWeapon->Destroy();
	}
}

void APlayerCharacter::AddItem(FString ItemCode)
{
	Inven.TryAddItem(ItemCode);
}


void APlayerCharacter::HideAllMeshComponent(bool Hide)
{
	HidePlayerMesh(Hide);
	HidePlayerEquipment(Hide);
}

void APlayerCharacter::HidePlayerMesh(bool Hide)
{
	Hide = !Hide;
	GetMesh()->SetVisibility(Hide);
	TArray<USceneComponent*> AttachedComponents;
	GetMesh()->GetChildrenComponents(true,  AttachedComponents);

	// Loop through all found Static Mesh components
	for (USceneComponent* SceneComponent : AttachedComponents)
	{
		UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(SceneComponent);
		if(SMComp)
		{
			SMComp->SetVisibility(Hide);
		}
	}
}

void APlayerCharacter::HidePlayerEquipment(bool Hide)
{
	if(!Combat) return;
	Hide = !Hide;
	if(Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
	{
		Combat->EquippedWeapon->GetWeaponMesh()->SetVisibility(Hide);
		TArray<USceneComponent*> AttachedComponents;
		Combat->EquippedWeapon->GetWeaponMesh()->GetChildrenComponents(true,  AttachedComponents);

		// Loop through all found Static Mesh components
		for (USceneComponent* SceneComponent : AttachedComponents)
		{
			UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(SceneComponent);
			if(SMComp)
			{
				SMComp->SetVisibility(Hide);
			}
		}
	}
	if(Combat->SecondaryWeapon && Combat->SecondaryWeapon->GetWeaponMesh())
	{
		Combat->SecondaryWeapon->GetWeaponMesh()->SetVisibility(Hide);
		TArray<USceneComponent*> AttachedComponents;
		Combat->SecondaryWeapon->GetWeaponMesh()->GetChildrenComponents(true,  AttachedComponents);

		// Loop through all found Static Mesh components
		for (USceneComponent* SceneComponent : AttachedComponents)
		{
			UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(SceneComponent);
			if(SMComp)
			{
				SMComp->SetVisibility(Hide);
			}
		}
	}
	if(EquippedBackpack)
	{
		EquippedBackpack -> GetItemMesh() -> SetVisibility(Hide);
	}
}

bool APlayerCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool APlayerCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

bool APlayerCharacter::IsLocallyReloading()
{
	if(Combat == nullptr) return false;
	return Combat->bLocallyReload;
}

AWeaponActor* APlayerCharacter::GetEquippedWeapon()
{
	if(Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

FVector APlayerCharacter::GetHitTarget() const
{
	if(Combat == nullptr) return FVector();
	return Combat->HitTarget;
}

ECombatState APlayerCharacter::GetCombatState() const
{
	if (Combat == nullptr) return ECombatState::ECS_MAX;
	return Combat->CombatState;
}


