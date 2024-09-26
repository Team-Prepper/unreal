// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerState.h"
#include "Prepper/Character/Component/Combat/CombatComponent.h"
#include "Prepper/HUD/PrepperHUD.h"
#include "Prepper/HUD/UI/Compass.h"


void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}
	SetPossessPawn();
	SetInputMode(FInputModeGameOnly());
	PrepperHUD = Cast<APrepperHUD>(GetHUD());
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	// 서버에서만 동작하는 함수
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<APlayerCharacter>(InPawn);
	SetPossessPawn();
	UE_LOG(LogTemp, Warning, TEXT("Pawn %s possessed by PlayerController %s"), *GetPawn()->GetName(), *GetName());
}

void ABasePlayerController::SetPossessPawn()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABasePlayerController::PossessNewPawn, 0.1f, true);
}

void ABasePlayerController::PossessNewPawn()
{
	if (!GetPawn()) return;

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	MulticastPossessNewPawn();
}

void ABasePlayerController::MulticastPossessNewPawn_Implementation()
{
	PossessPawn();
}

void ABasePlayerController::PossessPawn()
{
	// 로컬에서도 동작하게 설계함
	if (Cast<IControllable>(GetPawn()))
	{
		TargetControllerable = GetPawn();
	}

	if (!IsLocalController()) return;

	PollInit();
}

void ABasePlayerController::PollInit()
{
	PrepperHUD = PrepperHUD == nullptr ? Cast<APrepperHUD>(GetHUD()) : PrepperHUD;

	if (!PrepperHUD) return;
	if (!PrepperHUD->CharacterOverlay)
	{
		PrepperHUD->AddCharacterOverlay();
	}
	if (!PrepperHUD->CharacterOverlay)
	{
		return;
	}

	PrepperHUD->ResetCrossHair();

	if (PrepperHUD->Compass)
	{
		PrepperHUD->Compass->SetTargetCamera(TargetControllerable->GetFollowCamera());
		UE_LOG(LogTemp, Warning, TEXT("[PrepperPlayerController] : Set Compass"));
	}

	if (Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	}

	if (!PlayerCharacter) return;

	PossessPlayerCharacter();
}

void ABasePlayerController::PossessPlayerCharacter()
{
	PlayerCharacter->Attach(PrepperHUD->CharacterOverlay);
	PlayerCharacter->GetCombatComponent()->Attach(PrepperHUD->CharacterOverlay);
}

void ABasePlayerController::ResetPlayer()
{
	PlayerCharacter = nullptr;
}

void ABasePlayerController::ServerReportPingStatus_Implementation(bool bHighPing)
{
	HighPingDelegate.Broadcast(bHighPing);
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckPing(DeltaTime);
}

void ABasePlayerController::CheckPing(float DeltaTime)
{
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime <= CheckPingFrequency) return;

	if (!PlayerState)
	{
		PlayerState = GetPlayerState<APlayerState>();
	}

	UE_LOG(LogTemp, Warning, TEXT("Ping chk"));
	FString ping = FString::SanitizeFloat(PlayerState->GetPingInMilliseconds());
	UE_LOG(LogTemp, Warning, TEXT("PING : %s"), *ping);

	if (PlayerState && PlayerState->GetPingInMilliseconds() > HighPingThreshold)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIGH PING WARNING"));
		if (IsLocalController())
		{
			HighPingWarningBP();
			PingAnimationRunningTime = 0.f;
		}
	}
	HighPingRunningTime = 0.f;
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		SetInput(EnhancedInputComponent);
	}
}


void ABasePlayerController::SetInput(UEnhancedInputComponent* Input)
{
	// Moving
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
	Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABasePlayerController::Move);
	
	// Looking
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);

	// Jumping
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerController::JumpButtonPressed);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayerController::JumpButtonReleased);

	//Sprint
	Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ABasePlayerController::SprintButtonPressed);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayerController::SprintButtonReleased);

	// Equip
	Input->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ABasePlayerController::EquipButtonPressed);

	// Crouch
	Input->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABasePlayerController::CrouchButtonPressed);

	//Aim
	Input->BindAction(AimAction, ETriggerEvent::Triggered, this, &ABasePlayerController::AimButtonPressed);
	Input->BindAction(AimAction, ETriggerEvent::Completed, this, &ABasePlayerController::AimButtonReleased);

	// Fire
	Input->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABasePlayerController::FireButtonPressed);
	Input->BindAction(FireAction, ETriggerEvent::Completed, this, &ABasePlayerController::FireButtonReleased);

	// Reload
	Input->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ABasePlayerController::ReloadButtonPressed);
	
}

void ABasePlayerController::Move(const FInputActionValue& Value)
{
	if (!TargetControllerable) return;
	TargetControllerable->Move(Value);
}

void ABasePlayerController::Look(const FInputActionValue& Value)
{
	if (!TargetControllerable) return;
	TargetControllerable->Look(Value);
}

void ABasePlayerController::JumpButtonPressed()
{
	if (!TargetControllerable) return;
	TargetControllerable->SpacePressed();
}

void ABasePlayerController::JumpButtonReleased()
{
	if (!TargetControllerable) return;
	TargetControllerable->SpaceReleased();
}

void ABasePlayerController::SprintButtonPressed()
{
	if (!TargetControllerable) return;
	TargetControllerable->ShiftPressed();
}

void ABasePlayerController::SprintButtonReleased()
{
	if (!TargetControllerable) return;
	TargetControllerable->ShiftReleased();
}

void ABasePlayerController::EquipButtonPressed()
{
	ServerInteractionPressed();
}

void ABasePlayerController::CrouchButtonPressed()
{
	if (!TargetControllerable) return;
	TargetControllerable->ControlPressed();
}

void ABasePlayerController::ReloadButtonPressed()
{
	if (!TargetControllerable) return;
	TargetControllerable->RPressed();
}

void ABasePlayerController::AimButtonPressed()
{
	if (!TargetControllerable) return;
	TargetControllerable->MouseRightPressed();
}

void ABasePlayerController::AimButtonReleased()
{
	if (!TargetControllerable) return;
	TargetControllerable->MouseRightReleased();
}

void ABasePlayerController::FireButtonPressed()
{
	if (!TargetControllerable) return;
	TargetControllerable->MouseLeftPressed();
}

void ABasePlayerController::FireButtonReleased()
{
	if (!TargetControllerable) return;
	TargetControllerable->MouseLeftReleased();
}


void ABasePlayerController::ServerInteractionPressed_Implementation()
{
	if (!TargetControllerable) return;
	TargetControllerable->EPressed();
}
