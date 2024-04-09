
#include "AInteractable.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/Character/PlayerCharacter.h"


void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnSphereEndOverlap);
	
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(false);
	}
}

void AInteractable::ShowPickUpWidget(bool bShowWidget)
{
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(bShowWidget);
	}
}

void AInteractable::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingItem(this);
	}
}

void AInteractable::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingItem(nullptr);
	}
}