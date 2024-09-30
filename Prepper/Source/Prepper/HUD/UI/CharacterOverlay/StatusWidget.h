// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Prepper/_Base/ObserverPattern/Observer.h"
#include "StatusWidget.generated.h"

class Status;
class UProgressBar;
/**
 * 
 */

UCLASS()
class PREPPER_API UStatusWidget : public UUserWidget, public IObserver<Status>
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category=Story)
	UProgressBar* HungerBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category=Story)
	UProgressBar* ThirstBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category=Story)
	UProgressBar* InfectionBar;

public:
	virtual void Update(const Status& NewData) override;
	
};
