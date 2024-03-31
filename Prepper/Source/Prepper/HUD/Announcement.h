// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Announcement.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UAnnouncement : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WarmupTime;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;
};
