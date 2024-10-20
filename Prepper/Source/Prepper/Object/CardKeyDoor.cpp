// Fill out your copyright notice in the Description page of Project Settings.


#include "CardKeyDoor.h"

#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Item/Inventory/MapInventory.h"


// Sets default values
ACardKeyDoor::ACardKeyDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACardKeyDoor::Interaction(APlayerCharacter* Target)
{
	Super::Interaction(Target);
	if (Target->GetInventory()->TryGetItemCount(TargetItemCode) < 1) return;

	if (IsClosed)
	{
		OpenDoor();
		return;
	}
	
	CloseDoor();
}

