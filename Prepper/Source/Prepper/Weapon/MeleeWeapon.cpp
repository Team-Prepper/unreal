#include "MeleeWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AMeleeWeapon::AMeleeWeapon()
{
	MeleeWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeWeaponMesh"));
	MeleeWeaponMesh->SetupAttachment(RootComponent);
	
	WeaponTracer = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponTracer"));
	WeaponTracer->SetupAttachment(MeleeWeaponMesh);
	WeaponTracer->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponTracer->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponTracer->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);

	TracerStart = CreateDefaultSubobject<USceneComponent>(TEXT("TracerStart"));
	TracerStart-> SetupAttachment(MeleeWeaponMesh);
	TracerEnd   = CreateDefaultSubobject<USceneComponent>(TEXT("TracerEnd"));
	TracerEnd  -> SetupAttachment(MeleeWeaponMesh);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponTracer->OnComponentBeginOverlap.AddDynamic(this,&AMeleeWeapon::AMeleeWeapon::OnBoxOverlap);
}

void AMeleeWeapon::Fire(const FVector& HitTarget)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	FHitResult FireHit;
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(FireHit.GetActor());
	if (PlayerCharacter && HasAuthority() && InstigatorController)
	{
		UGameplayStatics::ApplyDamage(
			PlayerCharacter,
			Damage,
			InstigatorController,
			this,
			UDamageType::StaticClass()
		);
	}
	if(ImpactParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactParticles,
			FireHit.ImpactPoint,
			FireHit.ImpactNormal.Rotation()
		);
	}
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
		this,
		HitSound,
		FireHit.ImpactPoint);
	}
}


void AMeleeWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = TracerStart->GetComponentLocation();
	const FVector End = TracerEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		TracerStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true
	);
}
