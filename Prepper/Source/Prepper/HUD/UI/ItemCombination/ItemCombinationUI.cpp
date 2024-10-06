// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombinationUI.h"

#include "ItemCombinationSelectUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Prepper/Character/PlayerCharacter.h"
#include "Prepper/Item/ItemManager.h"
#include "Prepper/Item/Inventory/MapInventory.h"

void UItemCombinationUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SelectButton->OnClicked.AddDynamic(this, &UItemCombinationUI::OpenSelectWidget);
	CombinationButton->OnClicked.AddDynamic(this, &UItemCombinationUI::Combination);
	CloseButton->OnClicked.AddDynamic(this, &UItemCombinationUI::Close);
}

void UItemCombinationUI::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (TargetPlayer == nullptr) return;

	const TObjectPtr<APlayerController> PC =
		UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (PC == nullptr) return;
	
	if (InVisibility == ESlateVisibility::Visible)
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		SetFocus();
		
		return;
	}
	
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
	
}

void UItemCombinationUI::SetTargetPlayer(TObjectPtr<APlayerCharacter> Target)
{
	TargetPlayer = Target;
}

void UItemCombinationUI::SetCombinationTarget(const FString& CombinationTarget)
{
	TargetItem = CombinationTarget;
	
	UTexture2D* Img;
	FText Name;
	if (!ItemManager::GetInstance()->GetItemData(TargetItem, Img, Name)) return;

	CombinationResultIcon->SetBrushFromTexture(Img);

	ItemManager::GetInstance()->TryCombinationItem
	(TargetItem, SourceItem1, Cnt1,
		SourceItem2, Cnt2);
	
	if (!ItemManager::GetInstance()->GetItemData(SourceItem1, Img, Name)) return;
	CombinationSourceIcon1->SetBrushFromTexture(Img);
	if (!ItemManager::GetInstance()->GetItemData(SourceItem2, Img, Name)) return;
	CombinationSourceIcon2->SetBrushFromTexture(Img);

	
}
 
 void UItemCombinationUI::OpenSelectWidget()
 {
 	TObjectPtr<UItemCombinationSelectUI> Tmp =
 		CreateWidget<UItemCombinationSelectUI>(this, SelectWidget);
 
 	Tmp->AddToViewport();
 	Tmp->SetTarget(this);
 
 }

void UItemCombinationUI::Combination()
{
	
}

void UItemCombinationUI::Close()
{
	RemoveFromParent();
	
}
