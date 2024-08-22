// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElimEvent.h"
#include "Components/TimelineComponent.h"
#include "UObject/Object.h"
#include "ElimDissolveEvent.generated.h"

/**
 * 
 */
UCLASS()
class PREPPER_API UElimDissolveEvent : public UObject, public IElimEvent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	ACharacter* TargetCharacter;
	
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;
	// DissolveMaterialInstance로 부터 동적 생성
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// 블루 프린트에 세팅
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;
public:
	virtual void StartElim() override;
	virtual void SetTarget(ACharacter* Target) override;
	
	UFUNCTION()
	void UpdateDissolveMaterial(const float DissolveValue);
};
