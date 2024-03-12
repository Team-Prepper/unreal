#include "PlayerCharacter.h"	
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/Component/CombatComponent.h"
#include "Prepper/Item/Interactable.h"
#include "Prepper/Weapon/Weapon.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WalkSpeed = 600.f;
	SprintSpeed = 900.f;
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

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(APlayerCharacter, OverlappingItem, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APlayerCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->Character = this;
	}
}

void APlayerCharacter::PlayFireMontage(bool bAiming)
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);

	FVector CurrentLocation = CameraBoom->GetRelativeLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetSpringArmLocation, DeltaTime, InterpSpeed);
	CameraBoom->SetRelativeLocation(NewLocation);
	
	float CurrentArmLength = CameraBoom->TargetArmLength;
	float NewArmLength = FMath::FInterpTo(CurrentArmLength, TargetArmLength, DeltaTime, InterpSpeed);
	CameraBoom->TargetArmLength = NewArmLength;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SprintButtonPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintButtonReleased);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Equip
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EquipButtonPressed);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CrouchButtonPressed);

		//Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &APlayerCharacter::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::AimButtonReleased);

		// Fire
		//Aim
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APlayerCharacter::FireButtonPressed);
	}
}

void APlayerCharacter::SprintButtonPressed()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // local player
	ServerSprintButtonPressed(); // server
}

void APlayerCharacter::ServerSprintButtonPressed_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::SprintButtonReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	ServerSprintButtonReleased();
}

void APlayerCharacter::ServerSprintButtonReleased_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
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

void APlayerCharacter::EquipButtonPressed()
{
	if(OverlappingItem)
	{
		OverlappingItem->Interaction(this);
		return;
	}
	
	if(Combat)
	{
		if(HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}else
		{
			ServerEquipButtonPressed(OverlappingWeapon);
		}
	}
}

void APlayerCharacter::EquipWeapon(AWeapon* Weapon)
{
	if(Combat)
	{
		if(HasAuthority())
		{
			Combat->EquipWeapon(Weapon);
		}else
		{
			ServerEquipButtonPressed(Weapon);
		}
	}
	
	
}

void APlayerCharacter::ServerEquipButtonPressed_Implementation(AWeapon* Weapon)
{
	if(Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void APlayerCharacter::CrouchButtonPressed()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void APlayerCharacter::AimButtonPressed()
{
	if(Combat)
	{
		Combat->SetAiming(true);
	}
}

void APlayerCharacter::AimButtonReleased()
{
	if(Combat)
	{
		Combat->SetAiming(false);
	}
}

void APlayerCharacter::FireButtonPressed()
{
	if(Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void APlayerCharacter::FireButtonReleased()
{
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void APlayerCharacter::AimOffset(float DeltaTime)
{
	if(Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();

	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if(Speed == 0.f && !bIsInAir) // Standing, not Jumping
	{
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
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if(AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [ 270, 360) -> [-90,0)
		FVector2d InRange(270.f, 360.f);
		FVector2d OutRange(-90.f,0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
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

void APlayerCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(false);
	}
	OverlappingWeapon = Weapon;
	
	if(IsLocallyControlled())
	{
		if(OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickUpWidget(true);
		}
	}
}

void APlayerCharacter::SetOverlappingItem(AInteractable* InteractableItem)
{
	if(OverlappingItem)
	{
		OverlappingItem->ShowPickUpWidget(false);
	}
	OverlappingItem = InteractableItem;
	
	if(IsLocallyControlled())
	{
		if(OverlappingItem)
		{
			OverlappingItem->ShowPickUpWidget(true);
		}
	}
	
}

void APlayerCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(true);
	}
	if(LastWeapon)
	{
		LastWeapon->ShowPickUpWidget(false);
	}
}

void APlayerCharacter::OnRep_OverlappingItem(AInteractable* LastItem)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(true);
	}
	if(LastItem)
	{
		LastItem->ShowPickUpWidget(false);
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

AWeapon* APlayerCharacter::GetEquippedWeapon()
{
	if(Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

FVector APlayerCharacter::GetHitTarget() const
{
	if(Combat == nullptr) return FVector();
	return Combat->HitTarget;
}


