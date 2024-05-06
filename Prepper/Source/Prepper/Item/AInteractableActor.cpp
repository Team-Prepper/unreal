
#include "AInteractableActor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/Character/PlayerCharacter.h"


void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnSphereEndOverlap);
	
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(false);
	}
}

void AInteractableActor::ShowPickUpWidget(bool bShowWidget)
{
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(bShowWidget);
	}
}

void AInteractableActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingItem(this);
	}
}

void AInteractableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingItem(nullptr);
	}
}