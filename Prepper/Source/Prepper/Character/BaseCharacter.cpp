#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"

ABaseCharacter::ABaseCharacter()
{
	WalkSpeed = 600.f;
	SprintSpeed = 900.f;

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
}


void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, CurrentHealth);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUDHealth();
	if(HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::ReceiveDamage);
	}
}

void ABaseCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABaseCharacter::PlayHitReactMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("HitFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	UpdateHUDHealth();
	PlayHitReactMontage();
}

void ABaseCharacter::Elim()
{
	MulticastElim();
}

void ABaseCharacter::MulticastElim_Implementation()
{
	bElimmed = true;
	PlayElimMontage();

	// Start Dissolve Effect
	if (DissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		
		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);

		TArray<USceneComponent*> AttachedComponents;
		GetMesh()->GetChildrenComponents(true,  AttachedComponents);

		// Loop through all found Static Mesh components
		for (USceneComponent* SceneComponent : AttachedComponents)
		{
			UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(SceneComponent);
			if(SMComp)
			{
				SMComp->SetMaterial(0, DynamicDissolveMaterialInstance);
			}
		}
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("DissolveValue"), 0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("GlowValue"), 200.f);
	}
	StartDissolve();

	// Disable Movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	// Disable Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::UpdateDissolveMaterial(float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("DissolveValue"), DissolveValue);
	}
}

void ABaseCharacter::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &ABaseCharacter::UpdateDissolveMaterial);
	if (DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeline->Play();
	}
}

void ABaseCharacter::UpdateHUDHealth()
{
	PrepperPlayerController = PrepperPlayerController == nullptr ?  Cast<APrepperPlayerController>(Controller) : PrepperPlayerController;
	if(PrepperPlayerController)
	{
		PrepperPlayerController->SetHUDHealth(CurrentHealth, MaxHealth);
	}
}

void ABaseCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	PlayHitReactMontage();
}