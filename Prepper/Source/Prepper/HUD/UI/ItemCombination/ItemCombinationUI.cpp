// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCombinationUI.h"

#include "ItemCombinationSelectUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
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
	CntUp->OnClicked.AddDynamic(this, &UItemCombinationUI::CntUpAction);
	CntDown->OnClicked.AddDynamic(this, &UItemCombinationUI::CntDownAction);
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
		TargetPlayer->GetInventory()->Attach(this);
		SetFocus();
		
		return;
	}
	
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
	TargetPlayer->GetInventory()->Detach(this);
	
}

void UItemCombinationUI::Update(IInventory* const& newData)
{
	CntSet();
}

void UItemCombinationUI::SetTargetPlayer(TObjectPtr<APlayerCharacter> Target)
{
	TargetPlayer = Target;
	Cnt = 1;
}

void UItemCombinationUI::SetCombinationTarget(const FString& CombinationTarget)
{
	TargetItem = CombinationTarget;
	
	UTexture2D* Img;
	FText Name;
	if (!ItemManager::GetInstance()->GetItemData(TargetItem, Img, Name)) return;

	ResultIcon->SetBrushFromTexture(Img);

	ItemManager::GetInstance()->TryCombinationItem
	(TargetItem, SourceItem1, Cnt1,
		SourceItem2, Cnt2);
	
	if (!ItemManager::GetInstance()->GetItemData(SourceItem1, Img, Name)) return;

	SourceIcon1->SetBrushFromTexture(Img);
	
	if (!ItemManager::GetInstance()->GetItemData(SourceItem2, Img, Name)) return;
	
	SourceIcon2->SetBrushFromTexture(Img);

	CntSet();
	
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
	bool b = TargetPlayer->GetInventory()->TryUseItem(SourceItem1, Cnt1 * Cnt)
			&& TargetPlayer->GetInventory()->TryUseItem(SourceItem2, Cnt2 * Cnt);

	if (!b) return;
	
	TargetPlayer->GetInventory()->TryAddItem(TargetItem, Cnt);
}

void UItemCombinationUI::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
	
}

void UItemCombinationUI::CntUpAction()
{
	Cnt++;
	CntSet();
}

void UItemCombinationUI::CntDownAction()
{
	if (Cnt == 0) return;
	Cnt--;
	CntSet();
}

void UItemCombinationUI::CntSet()
{
	IInventory* Inventory = TargetPlayer->GetInventory();

	ResultCnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), Cnt)));
	
	SourceCnt1->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),
		Cnt1 * Cnt, Inventory->TryGetItemCount(SourceItem1))));
	SourceCnt2->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),
		Cnt2 * Cnt, Inventory->TryGetItemCount(SourceItem2))));
}