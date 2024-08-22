// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElimEvent.h"
#include "Components/TimelineComponent.h"
#include "ElimDissolveComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UElimDissolveComponent : public UActorComponent, public IElimEvent
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
	// Sets default values for this pawn's properties
	UElimDissolveComponent();
	
	virtual void StartElim() override;
	virtual void SetTarget(ACharacter* Target) override;

	UFUNCTION()
	void UpdateDissolveMaterial(const float DissolveValue);
};
