#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Prepper/PlayerController/PrepperPlayerController.h"
#include "Prepper/GameMode/DeathMatchGameMode.h"
#include "Prepper/_Base/Util/GaugeFloat.h"

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

void ABaseCharacter::AttachActorAtSocket(const FName& SocketName, AActor* TargetActor)
{
	const USkeletalMeshSocket* TargetSocket = GetMesh()->GetSocketByName(SocketName);
	if(TargetSocket)
	{
		TargetSocket->AttachActor(TargetActor, GetMesh());
	}
	UE_LOG(LogTemp, Warning, TEXT("Attach %s"), *SocketName.ToString());
}

void ABaseCharacter::Attach(IObserver<GaugeValue<float>>* Observer)
{
	Observers.insert(Observer);
	Observer->Update(FGaugeFloat(CurrentHealth, MaxHealth));
}

void ABaseCharacter::Detach(IObserver<GaugeValue<float>>* Observer)
{
	Observers.erase(Observer);
}

void ABaseCharacter::Notify()
{
	const FGaugeFloat Value(FGaugeFloat(CurrentHealth, MaxHealth));
	std::ranges::for_each(Observers, [&](IObserver<GaugeValue<float>>* Observer) {
		Observer->Update(Value);
	});
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &IDamageable::DynamicDamage);
	}
}

void ABaseCharacter::PlayElimMontage()
{
	PlayAnim(ElimMontage);
}

void ABaseCharacter::PlayHitReactMontage()
{
	PlayAnim(HitReactMontage, FName("HitFront"));
}

void ABaseCharacter::ReceiveDamage(float Damage, AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	PlayHitReactMontage();

	if (CurrentHealth < 0) CurrentHealth = 0;
	if(CurrentHealth != 0.f) return;
	
	// 해당 캐릭터가 사망했다면 
	APrepperGameMode* PrepperGameMode =  GetWorld()->GetAuthGameMode<APrepperGameMode>();
	
	if(PrepperGameMode == nullptr) return;
	
	PrepperPlayerController = (PrepperPlayerController == nullptr) ? Cast<APrepperPlayerController>(Controller) : PrepperPlayerController;
	APrepperPlayerController* AttackerController = Cast<APrepperPlayerController>(InstigatorController);
	PrepperGameMode->PlayerEliminated(this, PrepperPlayerController, AttackerController);
}

void ABaseCharacter::Elim()
{
	MulticastElim();
}

void ABaseCharacter::PlayAnim(UAnimMontage* Montage, const FName& SectionName)
{
	if (Montage == nullptr) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	AnimInstance->Montage_Play(Montage);
	if (SectionName.Compare("") == 0) return;
	
	AnimInstance->Montage_JumpToSection(SectionName);
}

void ABaseCharacter::MulticastElim_Implementation()
{
	bElimed = true;
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
	SetActorEnableCollision(false);
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

void ABaseCharacter::OnRep_Health()
{
	PlayHitReactMontage();
}