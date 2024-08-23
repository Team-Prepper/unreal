#include "OpenedInventory.h"

#include "GameFramework/SpringArmComponent.h"
#include "Prepper/Component/CustomCameraComponent.h"
#include "Prepper/GameInstance/PrepperGameInstance.h"
#include "Prepper/Item/Object/InventoryInteractableItem.h"

AOpenedInventory::AOpenedInventory()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BaseActor = CreateDefaultSubobject<USceneComponent>(TEXT("BaseActor"));
	SetRootComponent(BaseActor);
	
	InventoryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InventoryMesh"));
	InventoryMesh->SetupAttachment(RootComponent);
}

void AOpenedInventory::SetTargetInventory(UMapInventory* Inventory)
{
	TargetInventory = Inventory;
	InitInventory();
}

void AOpenedInventory::InitInventory()
{
	if(!HasAuthority()) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Opened Inven init"));
	UWorld* World = GetWorld();
	if(!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WORLD"));
		return;
	}
	FRotator SpawnRotation(0.0f, GetActorRotation().Yaw, 0.0f);
	
	TArray<IInventory::InventoryItem> Items = TargetInventory->GetIter();
	//UE_LOG(LogTemp, Warning, TEXT("Item Count : %d"), Items.Num());
	for (int i = 0; i < Items.Num(); i++)
	{
		IInventory::InventoryItem Item = Items[i]; // 생성할 아이템 선택

		UPrepperGameInstance* PrepperGameInstance = Cast<UPrepperGameInstance>(GetGameInstance());
		if(PrepperGameInstance->GetItemInstance(Item.ItemCode))
		{
			InventoryInteractableItem = World->SpawnActor<AInventoryInteractableItem>(
				*PrepperGameInstance->GetItemInstance(Item.ItemCode),
				GetActorLocation()
				+ GetActorForwardVector() * (RowPivot - 100.0f * (i % RowSize))
				+ GetActorRightVector() * (ColPivot + 100.0f * (i/RowSize))
				+ GetActorLocation().UpVector * HeightPivot,  
				SpawnRotation
				);
			if(InventoryInteractableItem)
			{
				SpawnedActors.Add(InventoryInteractableItem);	
			}
			UE_LOG(LogTemp, Warning, TEXT("Spawn Inven Item"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Check Game Instace!"));
		}
			
		InventoryInteractableItem->SetTargetInventory(TargetInventory);
	}
}

void AOpenedInventory::CloseInventory()
{
	for (auto Item : SpawnedActors)
	{
		Item->Destroy();
	}
}

