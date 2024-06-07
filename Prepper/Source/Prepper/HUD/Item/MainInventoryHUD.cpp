// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInventoryHUD.h"

#include "ItemGrid.h"

void UMainInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	ItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
	UE_LOG(LogTemp, Warning, TEXT("MAIN HUD Construct"));
}

void UMainInventoryHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ItemGrid = Cast<UItemGrid>(GetWidgetFromName("WBP_InvenGrid"));
	UE_LOG(LogTemp, Warning, TEXT("MAIN HUD INIT"));
}
