// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Prepper/Character/PlayerCharacter.h"

AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(ItemMesh);
	
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnSphereEndOverlap);
	

	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(false);
	}
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AInteractableObject::Interaction(APlayerCharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Open Door"));
	
}

void AInteractableObject::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if(PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingItem(this);
	}
}

void AInteractableObject::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingItem(nullptr);
	}
}

void AInteractableObject::ShowPickUpWidget(bool bShowWidget)
{
	if(PickUpWidget)
	{
		PickUpWidget->SetVisibility(bShowWidget);
	}
}